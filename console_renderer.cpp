#include "console_renderer.hpp"
#include "char_width_check.h"
#include <cstring>

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

	//스프라이트 콜
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

				//뒤집기 처리
				wchar_x = dc->Flip ? flipX : wchar_x;
				flipX--;
				flipX = flipX < 0 ? 0 : flipX;

				//널문자 및 공백 체크
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

				//깊이 버퍼 확인
				if (dc->SortingOrder > m_depthBuffer[screen_idx])
				{
					//드로잉 가능
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


	//문자열 콜 - 최상위 드로우
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

				//널문자 및 공백 체크
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