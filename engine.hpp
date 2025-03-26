#include <unordered_map>
#include "console_renderer.hpp"
#include "SceneManager.hpp"
#include "input.hpp"
#include "time.hpp"

#define GET_SCREEN_WIDTH Engine::GetInstance()->GetConsoleRenderer()->GetScreenWidth
#define GET_SCREEN_HEIGHT Engine::GetInstance()->GetConsoleRenderer()->GetScreenHeight

namespace MyGame
{
	class Engine
	{
	public:
		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		static bool Initialize(int screenWidth, int screenHeight);
		static Engine* GetInstance();

		ConsoleRenderer* GetConsoleRenderer();
		SceneManager* GetSceneManager();
		void Run();
		void Exit();
	private:
		static Engine* m_instance;
		Engine(int screenWidth, int screenHeight);

		bool m_engineIsExit;
		ConsoleRenderer m_consoleRenderer;
		SceneManager m_sceneManager;

		void Update();
		void Render();
	};
}