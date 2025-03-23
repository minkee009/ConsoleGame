#pragma once
#include <string>
#include <queue>
#include <Windows.h>
#include "sprite.h"

namespace MyGame
{
	class ConsoleRenderer
	{
	public:
		ConsoleRenderer(SHORT width, SHORT height);
		~ConsoleRenderer();

		int GetScreenWidth() { return m_width; }
		int GetScreenHeight() { return m_height; };
		const CHAR_INFO* GetScreen() { return m_screen; }

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
		CHAR_INFO* m_screen;
		SHORT* m_depthBuffer;
		SMALL_RECT m_window;
		HANDLE m_hConsole;

		std::queue<const SPRITE*> m_drawCalls;

		void Clear();
		void Draw();
		void Swap();
	};
}