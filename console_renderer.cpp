#include "console_renderer.hpp"
#include "char_width_check.h"
#include <cstring>

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
	m_depthBuffer = new SHORT[m_screenBufferSize];
	Clear();
}

MyGame::ConsoleRenderer::~ConsoleRenderer()
{
	for (size_t i = 0; i < SCREEN_BUFFER_MAXCOUNT; i++)
	{
		CloseHandle(m_screenBuffer[i]);
	}
	delete[] m_depthBuffer;
}

void MyGame::ConsoleRenderer::AddSpriteDrawCall(COORD pos,const SPRITE* sprite)
{
	m_spriteDrawCalls.emplace(pos, sprite);
}

void MyGame::ConsoleRenderer::AddStringDrawCall(COORD pos,const WCHAR* string)
{
	m_stringDrawCalls.emplace(pos, string);
}

void MyGame::ConsoleRenderer::Clear()
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(m_screenBuffer[m_screenBufferIndex], ' ', m_screenBufferSize, Coor, &dw);
	memset(m_depthBuffer, (SHORT)SHRT_MIN, sizeof(SHORT) * m_height * m_width);
}

void MyGame::ConsoleRenderer::Draw()
{
	//SetConsoleActiveScreenBuffer(m_hConsole);

	//��������Ʈ ��
	while (m_spriteDrawCalls.size() > 0)
	{
		COORD pos = m_spriteDrawCalls.front().first;
		const SPRITE* dc = m_spriteDrawCalls.front().second;

		SHORT biasX = pos.X - dc->Pivot.X - m_viewportX;
		SHORT biasY = pos.Y - dc->Pivot.Y - m_viewportY;

		SHORT minX = biasX < 0 ? 0 : biasX;
		SHORT maxX = biasX + dc->Size.X > m_width ? m_width : biasX + dc->Size.X;
		SHORT minY = biasY < 0 ? 0 : biasY;
		SHORT maxY = biasY + dc->Size.Y > m_height ? m_height : biasY + dc->Size.Y;

		for (int i = minY; i < maxY; i++)
		{
			auto flipX = maxX - minX - 1;

			for (int j = minX, cursorX = minX; j < maxX && cursorX < m_width; j++,cursorX++)
			{
				auto wchar_x = j - biasX;
				auto wchar_y = i - biasY;

				//������ ó��
				wchar_x = dc->Flip ? flipX : wchar_x;
				flipX--;
				flipX = flipX < 0 ? 0 : flipX;

				//�ι��� �� ���� üũ
				if (dc->ShapeString[wchar_y][wchar_x] == 0 || dc->ShapeString[wchar_y][wchar_x] == L' ')
					continue;

				COORD screenPos = { (cursorX), (i) };
				BOOL isWideFont = IsDoubleWidthCharacter(dc->ShapeString[wchar_y][wchar_x]);

				if (cursorX + isWideFont >= m_width)
				{
					cursorX += isWideFont;
					continue;
				}

				auto screen_idx = i * m_width + cursorX;
				BOOL	bRval = FALSE;
				DWORD	dwCharsWritten;

				//���� ���� Ȯ��
				if (dc->SortingOrder > m_depthBuffer[screen_idx])
				{
					//����� ����
					bRval = WriteConsoleOutputCharacterW(m_screenBuffer[m_screenBufferIndex], &dc->ShapeString[wchar_y][wchar_x], 1, screenPos, &dwCharsWritten);
					if (bRval == false) OutputDebugStringA("Error, WriteConsoleOutputCharacterW()\n");
					bRval = FillConsoleOutputAttribute(m_screenBuffer[m_screenBufferIndex], dc->Attribute, 1, screenPos, &dwCharsWritten);
					if (bRval == false) OutputDebugStringA("Error, FillConsoleOutputAttribute()\n");
					if (isWideFont && (cursorX + 1) < m_width)
					{
						m_depthBuffer[screen_idx + 1] = dc->SortingOrder;
					}

					m_depthBuffer[screen_idx] = dc->SortingOrder;

					cursorX += isWideFont;
				}
			}
		}

		m_spriteDrawCalls.pop();
	}


	//���ڿ� �� - �ֻ��� ��ο�
	while (m_stringDrawCalls.size() > 0)
	{
		COORD pos = m_stringDrawCalls.front().first;
		const WCHAR* dc = m_stringDrawCalls.front().second;
		auto sizeX = (int)(wcslen(dc) + 1);

		SHORT biasX = pos.X - m_viewportX;
		SHORT biasY = pos.Y - m_viewportY;

		SHORT minX = biasX < 0 ? 0 : biasX;
		SHORT maxX = biasX + sizeX > m_width ? m_width : biasX + sizeX;

		if (biasY <= m_height && biasY >= 0)
		{
			for (int j = minX, cursorX = minX; j < maxX && cursorX < m_width; j++, cursorX++)
			{
				auto wchar_x = j - biasX;

				//�ι��� �� ���� üũ
				if (dc[wchar_x] == 0 || dc[wchar_x] == L' ')
					continue;

				COORD screenPos = { (cursorX), (biasY) };
				BOOL isWideFont = IsDoubleWidthCharacter(dc[wchar_x]);

				if (cursorX + isWideFont >= m_width)
				{
					cursorX += isWideFont;
					continue;
				}

				auto screen_idx = biasY + cursorX;
				BOOL	bRval = FALSE;
				DWORD	dwCharsWritten;

				bRval = WriteConsoleOutputCharacterW(m_screenBuffer[m_screenBufferIndex], &dc[wchar_x], 1, screenPos, &dwCharsWritten);
				if (bRval == false) OutputDebugStringA("Error, WriteConsoleOutputCharacterW()\n");
				bRval = FillConsoleOutputAttribute(m_screenBuffer[m_screenBufferIndex], FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, 1, screenPos, &dwCharsWritten);
				if (bRval == false) OutputDebugStringA("Error, FillConsoleOutputAttribute()\n");

				cursorX += isWideFont;
			}
		}

		m_stringDrawCalls.pop();
	}
}

void MyGame::ConsoleRenderer::Swap()
{
	SetConsoleActiveScreenBuffer(m_screenBuffer[m_screenBufferIndex]);
	m_screenBufferIndex++;
	m_screenBufferIndex %= SCREEN_BUFFER_MAXCOUNT;
}