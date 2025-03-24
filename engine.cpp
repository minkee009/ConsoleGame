#include "engine.hpp"
#include "input.hpp"
#include "time.hpp"

MyGame::Engine::Engine(int screenWidth, int screenHeight) : m_consoleRenderer(screenWidth, screenHeight)
{
	m_engineIsExit = false;
}

MyGame::Engine::~Engine()
{

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
	//UPDATE_TIME();
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