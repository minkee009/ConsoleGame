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

		COORD GetViewPortAnchor() { return { m_viewportX, m_viewportY }; };

		void SetViewPortAnchor(SHORT x, SHORT y) { m_viewportX = x; m_viewportY = y; }

		void SpriteDraw(COORD pos, const SPRITE* sprite);
		void WStringDraw(COORD pos, const WCHAR* string);
		void WStringDraw(COORD pos, const WCHAR* string, int length);
		void WStringDrawColor(COORD pos, const WCHAR* string, WORD attribute);

		void Clear();
		void Swap();

	private:
		SHORT m_width;
		SHORT m_height;

		HANDLE m_screenBuffer[2];
		SHORT m_screenBufferIndex;
		SHORT m_screenBufferSize;
		SMALL_RECT m_window;
		HANDLE m_hConsole;

		SHORT m_viewportX;
		SHORT m_viewportY;
	};
}