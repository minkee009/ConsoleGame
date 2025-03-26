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

		void SetViewPortCenter(SHORT x, SHORT y) { m_viewportX = x; m_viewportY = y; }

		void AddSpriteDrawCall(COORD pos, const SPRITE* sprite);

		void AddStringDrawCall(COORD pos, const WCHAR* sprite);

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

		SHORT m_viewportX;
		SHORT m_viewportY;

		std::queue<std::pair<COORD, const SPRITE*>> m_spriteDrawCalls;
		std::queue<std::pair<COORD, const WCHAR*>> m_stringDrawCalls;

		void Clear();
		void Draw();
		void Swap();
	};
}