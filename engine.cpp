#include "engine.hpp"
#include <iostream>

std::unique_ptr<MyGame::Engine> MyGame::Engine::m_instance = nullptr;

MyGame::Engine::Engine(int screenWidth, int screenHeight) : m_consoleRenderer(screenWidth, screenHeight)
{
	m_engineIsExit = false;
	m_soundManager = new SoundManager;

	m_soundManager->LoadAllSoundsFromFolder("SE");
	m_soundManager->LoadAllSoundsFromFolder("BGM",true);
}

bool MyGame::Engine::Initialize(int screenWidth, int screenHeight)
{
	if (!m_instance) {
		try {
			m_instance = std::unique_ptr<Engine>(new Engine(screenWidth, screenHeight));
			return true;
		}
		catch (...) {
			return false;
		}
	}
	return true;
}

MyGame::Engine* MyGame::Engine::GetInstance()
{
	return m_instance.get();
}

MyGame::ConsoleRenderer* MyGame::Engine::GetConsoleRenderer()
{
	return &m_consoleRenderer;
}

MyGame::SceneManager* MyGame::Engine::GetSceneManager()
{
	return &m_sceneManager;
}

MyGame::SoundManager* MyGame::Engine::GetSoundManager()
{
	return m_soundManager;
}

void MyGame::Engine::Run()
{
	INIT_TIME();
	while (!m_engineIsExit)
	{
		Update();
		Render();
	}
}

void MyGame::Engine::Update()
{
	UPDATE_TIME();
	SCAN_KEY();
	m_sceneManager.CheckIsSceneChanged();
	m_sceneManager.UpdateScene();
	m_soundManager->Update();
}

void MyGame::Engine::Render()
{
	m_soundManager->Update();
	m_consoleRenderer.Clear();
	m_sceneManager.RenderScene();
	m_consoleRenderer.Swap();
}

void MyGame::Engine::Exit()
{
	m_engineIsExit = true;
}

MyGame::Engine::~Engine()
{
	delete m_soundManager;
}
