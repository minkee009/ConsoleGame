#include <vector>
#include "console_renderer.hpp"

namespace MyGame
{
	class Engine
	{
	public:
		Engine(int screenWidth, int screenHeight);
		~Engine();

		void Run();
		void Exit();
	private:
		
		bool m_engineIsExit;
		ConsoleRenderer m_consoleRenderer;

		void Update();
		void Render();
	};
}