#include "NinjaStar.hpp"
#include "engine.hpp"

MyGame::NinjaStar::NinjaStar(PlayScene* scene)
{
	m_scene = scene;

	m_spr =
	{
		{ NINJASTAR_SPR_SIZE_X, NINJASTAR_SPR_SIZE_Y },
		{ 0, 0 },
		m_goalFlagShape,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};
	m_posY = 10000;
}

void MyGame::NinjaStar::Initialize()
{
	m_posY = 10000;
}

void MyGame::NinjaStar::Update()
{
	m_posX += (m_dir ? -1 : 1) * NINJASTAR_SPEED * GET_DELTATIME();
}

void MyGame::NinjaStar::CallInteract(int collisionFlag)
{
	m_scene->gameState = PlayerDead;
}

