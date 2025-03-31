#include "BlockBroken.hpp"
#include "engine.hpp"
#include "Player.hpp"

MyGame::BlockBroken::BlockBroken(PlayScene* scene)
{
	m_scene = scene;

	m_spr =
	{
		{ BLOCK_BROKEN_SPR_SIZE_X, BLOCK_BROKEN_SPR_SIZE_Y },
		{ 0, 0 },
		m_blockShape,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};

	m_isSolid = false;
}

void MyGame::BlockBroken::Initialize()
{
	m_isSolid = false;
}

void MyGame::BlockBroken::Update()
{
	
	m_velY += PLAYER_GRAVITY * GET_DELTATIME();
	m_velY = min(PLAYER_MAXFALLSPEED, m_velY);

	m_posX += m_velX * GET_DELTATIME();
	m_posY += m_velY * GET_DELTATIME();
}

void MyGame::BlockBroken::CallInteract(int collisionFlag)
{
}
