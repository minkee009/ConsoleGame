#include "console_renderer.hpp"
#include <cstring>

MyGame::ConsoleRenderer::ConsoleRenderer(SHORT width, SHORT height)
{
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

void MyGame::ConsoleRenderer::AddDrawCall(const SPRITE* sprite)
{
	m_drawCalls.push(sprite);
}

void MyGame::ConsoleRenderer::Clear()
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(m_screenBuffer[m_screenBufferIndex], ' ', m_screenBufferSize, Coor, &dw);
	memset(m_depthBuffer, (SHORT)SHRT_MIN, sizeof(SHORT) * m_height * m_width);
}

bool MyGame::ConsoleRenderer::DrawChar(int x, int y, char ch, WORD attr)
{
	COORD	cdPos;
	BOOL	bRval = FALSE;
	DWORD	dwCharsWritten;
	cdPos.X = x;
	cdPos.Y = y;

	bRval = FillConsoleOutputCharacterA(m_screenBuffer[m_screenBufferIndex], ch, 1, cdPos, &dwCharsWritten);
	if (bRval == false) OutputDebugStringA("Error, FillConsoleOutputCharacter()\n");

	bRval = FillConsoleOutputAttribute(m_screenBuffer[m_screenBufferIndex], attr, 1, cdPos, &dwCharsWritten);
	if (bRval == false) OutputDebugStringA("Error, FillConsoleOutputAttribute()\n");
	return bRval;
}
bool MyGame::ConsoleRenderer::DrawChar(int x, int y, wchar_t ch, WORD attr)
{
	COORD	cdPos;
	BOOL	bRval = FALSE;
	DWORD	dwCharsWritten;
	cdPos.X = x;
	cdPos.Y = y;

	bRval = FillConsoleOutputCharacterW(m_screenBuffer[m_screenBufferIndex], ch, 1, cdPos, &dwCharsWritten);
	if (bRval == false) OutputDebugStringA("Error, FillConsoleOutputCharacter()\n");

	bRval = FillConsoleOutputAttribute(m_screenBuffer[m_screenBufferIndex], attr, 1, cdPos, &dwCharsWritten);
	if (bRval == false) OutputDebugStringA("Error, FillConsoleOutputAttribute()\n");
	return bRval;
}


bool MyGame::ConsoleRenderer::DrawString(int x, int y, const char* pStr, DWORD len, WORD attr)
{
	COORD	cdPos;
	BOOL	bRval = FALSE;
	DWORD	dwCharsWritten;
	cdPos.X = x;
	cdPos.Y = y;

	//특정 위치에 문자열을 출력한다.
	WriteConsoleOutputCharacterA(m_screenBuffer[m_screenBufferIndex], pStr, len, cdPos, &dwCharsWritten);
	bRval = FillConsoleOutputAttribute(m_screenBuffer[m_screenBufferIndex], attr, len, cdPos, &dwCharsWritten);
	if (bRval == false) OutputDebugStringA("Error, FillConsoleOutputAttribute()\n");
	return bRval;
}

bool MyGame::ConsoleRenderer::DrawString(int x, int y, const wchar_t* pStr, DWORD len, WORD attr)
{
	COORD	cdPos;
	BOOL	bRval = FALSE;
	DWORD	dwCharsWritten;
	cdPos.X = x;
	cdPos.Y = y;

	//특정 위치에 문자열을 출력한다.
	WriteConsoleOutputCharacterW(m_screenBuffer[m_screenBufferIndex], pStr, len, cdPos, &dwCharsWritten);
	bRval = FillConsoleOutputAttribute(m_screenBuffer[m_screenBufferIndex], attr, len, cdPos, &dwCharsWritten);
	if (bRval == false) OutputDebugStringA("Error, FillConsoleOutputAttribute()\n");
	return bRval;
}

void MyGame::ConsoleRenderer::Draw()
{
	while (m_drawCalls.size() > 0)
	{
		const SPRITE* dc = m_drawCalls.front();

		for (int i = 0; i < dc->Size.Y; i++)
			for (int j = 0; j < dc->Size.X; j++)
			{
				//범위 이탈
				if ((i - dc->Pivot.Y + dc->Position.Y) >= m_height
					|| (i - dc->Pivot.Y + dc->Position.Y) < 0
					|| (j - dc->Pivot.X + dc->Position.X) >= m_width
					|| (j - dc->Pivot.X + dc->Position.X) < 0)
					continue;

				auto pixel_idx = i * dc->Size.Y + j;

				//공백문자 체크
				if (dc->ShapeString[pixel_idx] == 0 || dc->ShapeString[pixel_idx] == L' ')
					continue;

				auto screen_x = (j + dc->Position.X - dc->Pivot.X);
				auto screen_y = (i - dc->Pivot.Y + dc->Position.Y);

				auto screen_idx = screen_y * m_width + screen_x;

				//깊이 버퍼 확인
				if (dc->SortingOrder > m_depthBuffer[screen_idx])
				{
					//드로잉 가능
					DrawChar(screen_x,screen_y,dc->ShapeString[pixel_idx], FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					m_depthBuffer[screen_idx] = dc->SortingOrder;
				}
			}

		m_drawCalls.pop();
	}
}

void MyGame::ConsoleRenderer::Swap()
{
	SetConsoleActiveScreenBuffer(m_screenBuffer[m_screenBufferIndex]);
	m_screenBufferIndex++;
	m_screenBufferIndex %= SCREEN_BUFFER_MAXCOUNT;
}