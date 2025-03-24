#pragma once
#include <string>
#include <queue>
#include <Windows.h>
#include "sprite.h"

#define SCREEN_BUFFER_MAXCOUNT 2

namespace MyGame
{
	class ConsoleRenderer
	{
	public:
		ConsoleRenderer(SHORT width, SHORT height);
		~ConsoleRenderer();

		int GetScreenWidth() { return m_width; }
		int GetScreenHeight() { return m_height; };

		void AddDrawCall(const SPRITE* sprite);

		void Render() 
		{
			Clear();
			Draw();
			Swap();
		}

	private:
		SHORT m_width;
		SHORT m_height;

		HANDLE m_screenBuffer[2];
		SHORT m_screenBufferIndex;
		SHORT m_screenBufferSize;
		SHORT* m_depthBuffer;
		SMALL_RECT m_window;
		HANDLE m_hConsole;

		std::queue<const SPRITE*> m_drawCalls;

		bool DrawChar(int x, int y, char ch, WORD attr);
		bool DrawChar(int x, int y, wchar_t ch, WORD attr);
		bool DrawString(int x, int y, const char* pStr, DWORD len, WORD attr);
		bool DrawString(int x, int y, const wchar_t* pStr, DWORD len, WORD attr);

		void Clear();
		void Draw();
		void Swap();
	};
}