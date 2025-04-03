#include <unordered_map>
#include <memory>
#include "console_renderer.hpp"
#include "scene_manager.hpp"
#include "input.hpp"
#include "time.hpp"
#include "sound_manager.hpp"

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
		SoundManager* GetSoundManager();
		void Run();
		void Exit();

		~Engine();
	private:
		static std::unique_ptr<Engine> m_instance;
		Engine(int screenWidth, int screenHeight);

		bool m_engineIsExit;
		ConsoleRenderer m_consoleRenderer;
		SceneManager m_sceneManager;
		SoundManager* m_soundManager;


		void Update();
		void Render();
	};
}

//Console Renderer
#define GET_SCREEN_WIDTH Engine::GetInstance()->GetConsoleRenderer()->GetScreenWidth
#define GET_SCREEN_HEIGHT Engine::GetInstance()->GetConsoleRenderer()->GetScreenHeight
#define GET_SCREEN_BBOX Engine::GetInstance()->GetConsoleRenderer()->GetViewPortBbox
#define GET_ANCHOR_POS Engine::GetInstance()->GetConsoleRenderer()->GetViewPortAnchor
#define SET_ANCHOR_POS Engine::GetInstance()->GetConsoleRenderer()->SetViewPortAnchor
#define RENDER_SPR Engine::GetInstance()->GetConsoleRenderer()->SpriteDraw
#define RENDER_STR Engine::GetInstance()->GetConsoleRenderer()->WStringDraw
#define RENDER_STR_COLOR Engine::GetInstance()->GetConsoleRenderer()->WStringDrawColor

//Scnene Manager
#define ADD_SCENE Engine::GetInstance()->GetSceneManager()->AddScene
#define CHANGE_SCENE Engine::GetInstance()->GetSceneManager()->ChangeScene
#define GET_SCENE Engine::GetInstance()->GetSceneManager()->GetScene

//Sound Manager
#define PLAY_BGM Engine::GetInstance()->GetSoundManager()->PlayBGM
#define PLAY_SE Engine::GetInstance()->GetSoundManager()->PlaySE
#define STOP_BGM Engine::GetInstance()->GetSoundManager()->StopBGM
#define BGM_ISPLAYING Engine::GetInstance()->GetSoundManager()->IsBGMPlaying
#define PAUSE_BGM Engine::GetInstance()->GetSoundManager()->PauseBGM

//Engine
#define ENGINE_EXIT Engine::GetInstance()->Exit