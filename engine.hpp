#include <unordered_map>
#include <memory>
#include "console_renderer.hpp"
#include "scene_manager.hpp"
#include "input.hpp"
#include "time.hpp"

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
		static std::unique_ptr<Engine> m_instance;
		Engine(int screenWidth, int screenHeight);

		bool m_engineIsExit;
		ConsoleRenderer m_consoleRenderer;
		SceneManager m_sceneManager;

		void Update();
		void Render();
	};
}

//Console Renderer
#define GET_SCREEN_WIDTH Engine::GetInstance()->GetConsoleRenderer()->GetScreenWidth
#define GET_SCREEN_HEIGHT Engine::GetInstance()->GetConsoleRenderer()->GetScreenHeight
#define GET_ANCHOR_POS Engine::GetInstance()->GetConsoleRenderer()->GetViewPortAnchor
#define SET_ANCHOR_POS Engine::GetInstance()->GetConsoleRenderer()->SetViewPortAnchor
#define RENDER_SPR Engine::GetInstance()->GetConsoleRenderer()->SpriteDraw
#define RENDER_STR Engine::GetInstance()->GetConsoleRenderer()->WStringDraw
#define RENDER_STR_COLOR Engine::GetInstance()->GetConsoleRenderer()->WStringDrawColor

//Scnene Manager
#define ADD_SCENE Engine::GetInstance()->GetSceneManager()->AddScene
#define CHANGE_SCENE Engine::GetInstance()->GetSceneManager()->ChangeScene
#define GET_SCENE Engine::GetInstance()->GetSceneManager()->GetScene

//Engine
#define ENGINE_EXIT Engine::GetInstance()->Exit