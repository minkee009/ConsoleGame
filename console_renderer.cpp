#include "console_renderer.hpp"
#include <cstring>

MyGame::ConsoleRenderer::ConsoleRenderer(SHORT width, SHORT height)
{
	m_width = width;
	m_height = height;

	COORD bufferSize = { m_width, m_height };

	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	m_window = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(m_hConsole, TRUE, &m_window);

	SetConsoleScreenBufferSize(m_hConsole, bufferSize);
	SetConsoleActiveScreenBuffer(m_hConsole);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(m_hConsole, &cursorInfo); // 현재 커서 정보 가져오기
	cursorInfo.bVisible = FALSE;                // 커서 숨기기
	SetConsoleCursorInfo(m_hConsole, &cursorInfo); // 설정 적용

	// Set the font size now that the screen buffer has been assigned to the console
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(m_hConsole, &csbi);

	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	m_screen = new CHAR_INFO[m_height * m_width];
	m_depthBuffer = new SHORT[m_height * m_width];
	Clear();

	m_window = { 0, 0, (SHORT)(m_width - 1), (SHORT)(m_height - 1) };
	if (!SetConsoleWindowInfo(m_hConsole, TRUE, &m_window))
		return;
}

MyGame::ConsoleRenderer::~ConsoleRenderer()
{
	delete[] m_screen;
	delete[] m_depthBuffer;
}

void MyGame::ConsoleRenderer::AddDrawCall(const SPRITE* sprite)
{
	m_drawCalls.push(sprite);
}

void MyGame::ConsoleRenderer::Clear()
{
	memset(m_screen, 0, sizeof(CHAR_INFO) * m_height * m_width);
	memset(m_depthBuffer, (SHORT)SHRT_MIN, sizeof(SHORT) * m_height * m_width);
}

void MyGame::ConsoleRenderer::Draw()
{
	while (m_drawCalls.size() > 0)
	{
		const SPRITE* dc = m_drawCalls.front();

		for(int i = 0; i < dc->Size.Y; i++)
			for (int j = 0; j < dc->Size.X; j++)
			{
				//범위 이탈
				if ( (i - dc->Pivot.Y + dc->Position.Y) >= m_height
					|| (i - dc->Pivot.Y + dc->Position.Y) < 0
					|| (j - dc->Pivot.X + dc->Position.X) >= m_width
					|| (j - dc->Pivot.X + dc->Position.X) < 0)
					continue;

				auto pixel_idx = i * dc->Size.Y + j;

				//공백문자 체크
				if (dc->Pixels[pixel_idx].Char.UnicodeChar == 0)
					continue;

				auto screen_idx = (i - dc->Pivot.Y + dc->Position.Y) * m_width + (j + dc->Position.X - dc->Pivot.X);

				//깊이 버퍼 확인
				if (dc->SortingOrder > m_depthBuffer[screen_idx])
				{
					//드로잉 가능
					m_screen[screen_idx] = dc->Pixels[pixel_idx];
					m_depthBuffer[screen_idx] = dc->SortingOrder;
				}
			}

		m_drawCalls.pop();
	}

	WriteConsoleOutput(m_hConsole, m_screen, { m_width, m_height }, { 0,0 }, &m_window);
}

void MyGame::ConsoleRenderer::Swap()
{

}