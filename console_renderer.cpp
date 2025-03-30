#include "console_renderer.hpp"
#include "char_check.h"
#include <cstring>
#include <algorithm> 

MyGame::ConsoleRenderer::ConsoleRenderer(SHORT width, SHORT height)
{
	HWND hwnd = GetConsoleWindow();
	if (hwnd == NULL) return;

	// 현재 윈도우 스타일 가져오기
	LONG style = GetWindowLong(hwnd, GWL_STYLE);

	// 크기 조정 및 최대화 스타일 제거
	style &= ~WS_SIZEBOX;       // 크기 조정 막기
	style &= ~WS_MAXIMIZEBOX;   // 최대화 버튼 제거

	// 수정된 스타일 적용
	SetWindowLong(hwnd, GWL_STYLE, style);

	// 창 위치와 크기 고정
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

	// 콘솔 창 크기 조정을 방해할 수 있는 기존 창 설정 제거
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(m_hConsole, &csbi);

	// 콘솔 창 크기를 최소화 (강제로 이동 후 축소)
	SMALL_RECT tempWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(m_hConsole, TRUE, &tempWindow);

	// 콘솔 버퍼 크기 설정
	SetConsoleScreenBufferSize(m_hConsole, bufferSize);

	// 콘솔 창 크기 확대
	m_window = { 0, 0, (SHORT)(m_width - 1), (SHORT)(m_height - 1) };
	SetConsoleWindowInfo(m_hConsole, TRUE, &m_window);

	// 스크린 버퍼 생성 (버퍼 크기 설정 후 생성)
	for (size_t i = 0; i < SCREEN_BUFFER_MAXCOUNT; i++)
	{
		m_screenBuffer[i] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(m_screenBuffer[i], bufferSize);
	}

	// 커서 숨기기
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.bVisible = FALSE;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(m_hConsole, &cursorInfo);
	for (size_t i = 0; i < SCREEN_BUFFER_MAXCOUNT; i++)
	{
		SetConsoleCursorInfo(m_screenBuffer[i], &cursorInfo);
	}

	// 콘솔 모드 설정
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	// 출력 버퍼 초기화
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

	// Y축 완전히 벗어난 경우 그리지 않음
	if (biasY < 0 || biasY >= m_height)
	{
		OutputDebugStringA("Y-axis out of bounds\n");
		return;
	}

	// X축 클리핑
	SHORT startX = pos.X - m_viewportX;
	SHORT clippedLength = length;

	// 왼쪽 클리핑
	if (startX < 0)
	{
		clippedLength += startX;
		if (clippedLength <= 0)
		{
			return; // 클리핑된 길이가 0 이하이면 그리지 않음
		}
		string -= startX;
		startX = 0;
	}

	// 오른쪽 클리핑 (한 줄을 넘어가지 않도록)
	if (startX + clippedLength > m_width)
	{
		clippedLength = m_width - startX;
	}

	// 클리핑된 길이가 0 이하면 그리지 않음
	if (clippedLength <= 0)
	{
		return;
	}

	BOOL bRval = FALSE;
	DWORD dwCharsWritten;

	// 클리핑된 문자열과 좌표로 출력
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

	// Y축 완전히 벗어난 경우 그리지 않음
	if (biasY < 0 || biasY >= m_height)
	{
		OutputDebugStringA("Y-axis out of bounds\n");
		return;
	}

	// X축 클리핑
	SHORT startX = pos.X - m_viewportX;
	SHORT clippedLength = length;

	// 왼쪽 클리핑
	if (startX < 0)
	{
		clippedLength += startX;
		if (clippedLength <= 0)
		{
			OutputDebugStringA("Left clipping resulted in zero or negative length\n");
			return; // 클리핑된 길이가 0 이하이면 그리지 않음
		}
		string -= startX;
		startX = 0;
	}

	// 오른쪽 클리핑 (한 줄을 넘어가지 않도록)
	if (startX + clippedLength > m_width)
	{
		clippedLength = m_width - startX;
	}

	// 클리핑된 길이가 0 이하면 그리지 않음
	if (clippedLength <= 0)
	{
		OutputDebugStringA("Right clipping resulted in zero or negative length\n");
		return;
	}

	BOOL bRval = FALSE;
	DWORD dwCharsWritten;

	// 클리핑된 문자열과 좌표로 출력
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

	// Y축 완전히 벗어난 경우 그리지 않음
	if (biasY < 0 || biasY >= m_height)
	{
		OutputDebugStringA("Y-axis out of bounds\n");
		return;
	}

	// X축 클리핑
	SHORT startX = pos.X - m_viewportX;
	SHORT clippedLength = length;

	// 왼쪽 클리핑
	if (startX < 0)
	{
		clippedLength += startX;
		if (clippedLength <= 0)
		{
			return; // 클리핑된 길이가 0 이하이면 그리지 않음
		}
		string -= startX;
		startX = 0;
	}

	// 오른쪽 클리핑 (한 줄을 넘어가지 않도록)
	if (startX + clippedLength > m_width)
	{
		clippedLength = m_width - startX;
	}

	// 클리핑된 길이가 0 이하면 그리지 않음
	if (clippedLength <= 0)
	{
		return;
	}

	BOOL bRval = FALSE;
	DWORD dwCharsWritten;

	// 클리핑된 문자열과 좌표로 출력
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
