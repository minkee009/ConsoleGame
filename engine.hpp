#include <vector>
#include "console_renderer.hpp"

namespace MyGame
{
	class Engine
	{
	public:
		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		static bool Initialize(int screenWidth, int screenHeight);
		static Engine* GetInstance();

		const ConsoleRenderer* GetConsoleRenderer();
		void Run();
		void Exit();
	private:
		static Engine* m_instance;
		Engine(int screenWidth, int screenHeight);

		bool m_engineIsExit;
		ConsoleRenderer m_consoleRenderer;

		void Update();
		void Render();
	};
}