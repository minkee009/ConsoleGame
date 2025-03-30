#include "console_renderer.hpp"
#include "char_check.h"
#include <cstring>
#include <algorithm> 

MyGame::ConsoleRenderer::ConsoleRenderer(SHORT width, SHORT height)
{
	HWND hwnd = GetConsoleWindow();
	if (hwnd == NULL) return;

	// ���� ������ ��Ÿ�� ��������
	LONG style = GetWindowLong(hwnd, GWL_STYLE);

	// ũ�� ���� �� �ִ�ȭ ��Ÿ�� ����
	style &= ~WS_SIZEBOX;       // ũ�� ���� ����
	style &= ~WS_MAXIMIZEBOX;   // �ִ�ȭ ��ư ����

	// ������ ��Ÿ�� ����
	SetWindowLong(hwnd, GWL_STYLE, style);

	// â ��ġ�� ũ�� ����
	SetWindowPos(
		hwnd,
		NULL,
		0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
	);

	m_width = width;
	m_height = height;

	COORD bufferSize = { m_width, m_height };
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	m_screenBufferIndex = 0;

	// �ܼ� â ũ�� ������ ������ �� �ִ� ���� â ���� ����
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(m_hConsole, &csbi);

	// �ܼ� â ũ�⸦ �ּ�ȭ (������ �̵� �� ���)
	SMALL_RECT tempWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(m_hConsole, TRUE, &tempWindow);

	// �ܼ� ���� ũ�� ����
	SetConsoleScreenBufferSize(m_hConsole, bufferSize);

	// �ܼ� â ũ�� Ȯ��
	m_window = { 0, 0, (SHORT)(m_width - 1), (SHORT)(m_height - 1) };
	SetConsoleWindowInfo(m_hConsole, TRUE, &m_window);

	// ��ũ�� ���� ���� (���� ũ�� ���� �� ����)
	for (size_t i = 0; i < SCREEN_BUFFER_MAXCOUNT; i++)
	{
		m_screenBuffer[i] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(m_screenBuffer[i], bufferSize);
	}

	// Ŀ�� �����
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.bVisible = FALSE;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(m_hConsole, &cursorInfo);
	for (size_t i = 0; i < SCREEN_BUFFER_MAXCOUNT; i++)
	{
		SetConsoleCursorInfo(m_screenBuffer[i], &cursorInfo);
	}

	// �ܼ� ��� ����
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	// ��� ���� �ʱ�ȭ
	m_screenBufferSize = m_height * m_width;
	Clear();
}

MyGame::ConsoleRenderer::~ConsoleRenderer()
{
	for (size_t i = 0; i < SCREEN_BUFFER_MAXCOUNT; i++)
	{
		CloseHandle(m_screenBuffer[i]);
	}
}

void MyGame::ConsoleRenderer::SpriteDraw(COORD pos, const SPRITE* sprite)
{
	SHORT biasY = pos.Y - sprite->Pivot.Y - m_viewportY;
	SHORT minY = max((SHORT)0, -biasY);
	SHORT maxY = min(sprite->Size.Y, m_height - biasY);

	SHORT biasX = pos.X - sprite->Pivot.X - m_viewportX;
	SHORT minX = max((SHORT)0, -biasX);
	SHORT maxX = min(sprite->Size.X, m_width - biasX);

	for (int y = minY; y < maxY; ++y)
	{
		BOOL bRval = FALSE;
		DWORD dwCharsWritten;

		// Calculate the visible range in the sprite's row
		SHORT startX = max((SHORT)0, -biasX);
		SHORT endX = min(sprite->Size.X, m_width - biasX);

		if (startX < endX)
		{
			std::wstring rowToDraw;
			if (sprite->Flip)
			{
				for (int x = sprite->Size.X - 1; x >= 0; --x)
				{
					wchar_t ch = sprite->ShapeString[y][x];
					if (isSpecialCharacter(ch))
					{
						rowToDraw.push_back(*flipSpecialCharacter(ch));
					}
					else
					{
						rowToDraw.push_back(ch);
					}
				}

				// Adjust the rowToDraw to fit within the screen boundaries
				if (biasX < 0)
				{
					rowToDraw = rowToDraw.substr(-biasX, rowToDraw.size() + biasX);
				}
				else if (biasX + sprite->Size.X > m_width)
				{
					rowToDraw = rowToDraw.substr(0, m_width - biasX);
				}
			}
			else
			{
				rowToDraw.assign(sprite->ShapeString[y] + startX, sprite->ShapeString[y] + endX);
			}

			// Write the visible part of the (possibly flipped) sprite's row
			bRval = WriteConsoleOutputCharacterW(
				m_screenBuffer[m_screenBufferIndex],
				rowToDraw.c_str(),
				rowToDraw.size(),
				{ (SHORT)(biasX + startX), (SHORT)(biasY + y) },
				&dwCharsWritten
			);
			if (bRval == FALSE) OutputDebugStringA("Error, WriteConsoleOutputCharacterW()\n");

			bRval = FillConsoleOutputAttribute(
				m_screenBuffer[m_screenBufferIndex],
				sprite->Attribute,
				rowToDraw.size(),
				{ (SHORT)(biasX + startX), (SHORT)(biasY + y) },
				&dwCharsWritten
			);
			if (bRval == FALSE) OutputDebugStringA("Error, FillConsoleOutputAttribute()\n");
		}
	}
}


void MyGame::ConsoleRenderer::WStringDraw(COORD pos, const WCHAR* string)
{
	auto length = (int)(wcslen(string));
	SHORT biasY = pos.Y - m_viewportY;

	// Y�� ������ ��� ��� �׸��� ����
	if (biasY < 0 || biasY >= m_height)
	{
		OutputDebugStringA("Y-axis out of bounds\n");
		return;
	}

	// X�� Ŭ����
	SHORT startX = pos.X - m_viewportX;
	SHORT clippedLength = length;

	// ���� Ŭ����
	if (startX < 0)
	{
		clippedLength += startX;
		if (clippedLength <= 0)
		{
			return; // Ŭ���ε� ���̰� 0 �����̸� �׸��� ����
		}
		string -= startX;
		startX = 0;
	}

	// ������ Ŭ���� (�� ���� �Ѿ�� �ʵ���)
	if (startX + clippedLength > m_width)
	{
		clippedLength = m_width - startX;
	}

	// Ŭ���ε� ���̰� 0 ���ϸ� �׸��� ����
	if (clippedLength <= 0)
	{
		return;
	}

	BOOL bRval = FALSE;
	DWORD dwCharsWritten;

	// Ŭ���ε� ���ڿ��� ��ǥ�� ���
	bRval = WriteConsoleOutputCharacterW(
		m_screenBuffer[m_screenBufferIndex],
		string,
		clippedLength,
		{ startX, biasY },
		&dwCharsWritten
	);
	if (!bRval) OutputDebugStringA("Error, WriteConsoleOutputCharacterW()\n");

	bRval = FillConsoleOutputAttribute(
		m_screenBuffer[m_screenBufferIndex],
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		clippedLength,
		{ startX, biasY },
		&dwCharsWritten
	);
	if (!bRval) OutputDebugStringA("Error, FillConsoleOutputAttribute()\n");
}
void MyGame::ConsoleRenderer::WStringDraw(COORD pos, const WCHAR* string, int length)
{
	SHORT biasY = pos.Y - m_viewportY;

	// Y�� ������ ��� ��� �׸��� ����
	if (biasY < 0 || biasY >= m_height)
	{
		OutputDebugStringA("Y-axis out of bounds\n");
		return;
	}

	// X�� Ŭ����
	SHORT startX = pos.X - m_viewportX;
	SHORT clippedLength = length;

	// ���� Ŭ����
	if (startX < 0)
	{
		clippedLength += startX;
		if (clippedLength <= 0)
		{
			OutputDebugStringA("Left clipping resulted in zero or negative length\n");
			return; // Ŭ���ε� ���̰� 0 �����̸� �׸��� ����
		}
		string -= startX;
		startX = 0;
	}

	// ������ Ŭ���� (�� ���� �Ѿ�� �ʵ���)
	if (startX + clippedLength > m_width)
	{
		clippedLength = m_width - startX;
	}

	// Ŭ���ε� ���̰� 0 ���ϸ� �׸��� ����
	if (clippedLength <= 0)
	{
		OutputDebugStringA("Right clipping resulted in zero or negative length\n");
		return;
	}

	BOOL bRval = FALSE;
	DWORD dwCharsWritten;

	// Ŭ���ε� ���ڿ��� ��ǥ�� ���
	bRval = WriteConsoleOutputCharacterW(
		m_screenBuffer[m_screenBufferIndex],
		string,
		clippedLength,
		{ startX, biasY },
		&dwCharsWritten
	);
	if (bRval == false) OutputDebugStringA("Error, WriteConsoleOutputCharacterW()\n");

	bRval = FillConsoleOutputAttribute(
		m_screenBuffer[m_screenBufferIndex],
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		clippedLength,
		{ startX, biasY },
		&dwCharsWritten
	);
	if (bRval == false) OutputDebugStringA("Error, FillConsoleOutputAttribute()\n");
}

void MyGame::ConsoleRenderer::WStringDrawColor(COORD pos, const WCHAR* string, WORD attribute)
{
	auto length = (int)(wcslen(string));
	SHORT biasY = pos.Y - m_viewportY;

	// Y�� ������ ��� ��� �׸��� ����
	if (biasY < 0 || biasY >= m_height)
	{
		OutputDebugStringA("Y-axis out of bounds\n");
		return;
	}

	// X�� Ŭ����
	SHORT startX = pos.X - m_viewportX;
	SHORT clippedLength = length;

	// ���� Ŭ����
	if (startX < 0)
	{
		clippedLength += startX;
		if (clippedLength <= 0)
		{
			return; // Ŭ���ε� ���̰� 0 �����̸� �׸��� ����
		}
		string -= startX;
		startX = 0;
	}

	// ������ Ŭ���� (�� ���� �Ѿ�� �ʵ���)
	if (startX + clippedLength > m_width)
	{
		clippedLength = m_width - startX;
	}

	// Ŭ���ε� ���̰� 0 ���ϸ� �׸��� ����
	if (clippedLength <= 0)
	{
		return;
	}

	BOOL bRval = FALSE;
	DWORD dwCharsWritten;

	// Ŭ���ε� ���ڿ��� ��ǥ�� ���
	bRval = WriteConsoleOutputCharacterW(
		m_screenBuffer[m_screenBufferIndex],
		string,
		clippedLength,
		{ startX, biasY },
		&dwCharsWritten
	);
	if (!bRval) OutputDebugStringA("Error, WriteConsoleOutputCharacterW()\n");

	bRval = FillConsoleOutputAttribute(
		m_screenBuffer[m_screenBufferIndex],
		attribute,
		clippedLength,
		{ startX, biasY },
		&dwCharsWritten
	);
	if (!bRval) OutputDebugStringA("Error, FillConsoleOutputAttribute()\n");
}

void MyGame::ConsoleRenderer::Clear()
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(m_screenBuffer[m_screenBufferIndex], ' ', m_screenBufferSize, Coor, &dw);
}

void MyGame::ConsoleRenderer::Swap()
{
	SetConsoleActiveScreenBuffer(m_screenBuffer[m_screenBufferIndex]);
	m_screenBufferIndex++;
	m_screenBufferIndex %= SCREEN_BUFFER_MAXCOUNT;
}
