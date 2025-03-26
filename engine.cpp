#include "engine.hpp"
#include "input.hpp"
#include "time.hpp"

MyGame::Engine* MyGame::Engine::m_instance = nullptr;

MyGame::Engine::Engine(int screenWidth, int screenHeight) : m_consoleRenderer(screenWidth, screenHeight)
{
	m_engineIsExit = false;
}

bool MyGame::Engine::Initialize(int screenWidth, int screenHeight)
{
	if (!m_instance) {
		try {
			m_instance = new Engine(screenWidth, screenHeight);
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
	return m_instance;
}

const MyGame::ConsoleRenderer* MyGame::Engine::GetConsoleRenderer()
{
	return &m_consoleRenderer;
}

void MyGame::Engine::Run()
{
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
}

void MyGame::Engine::Render()
{
	m_consoleRenderer.Render();
}

void MyGame::Engine::Exit()
{
	m_engineIsExit = true;
}