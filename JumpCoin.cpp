#include "JumpCoin.hpp"
#include "engine.hpp"
#include "math.hpp"

MyGame::JumpCoin::JumpCoin(PlayScene* scene)
{
	m_scene = scene;
	m_isInstancePlay = true;
	m_spr =
	{
		{JUMPCOIN_SPR_SIZE_X,JUMPCOIN_SPR_SIZE_Y},
		{ 0, 0 },
		m_jumpCoinShape1,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};
}

void MyGame::JumpCoin::Initialize()
{
	m_isAlive = true;
	m_velY = -25.0f;
	m_timer = 0.0f;
	m_instanceTimer = 0.0f;
	m_spr.ShapeString = m_jumpCoinShape1;
}

void MyGame::JumpCoin::Update()
{
	m_timer += GET_DELTATIME();
	m_instanceTimer += GET_DELTATIME();
	if (m_timer > 0.04f)
	{
		m_timer = 0.0f;
		m_sprIdx++;
		m_sprIdx %= JUMPCOIN_SPR_ANIM_MAXFRAME;
		switch (m_sprIdx)
		{
		case 0:
			m_spr.ShapeString = m_jumpCoinShape1;
			break;
		case 1:
			m_spr.ShapeString = m_jumpCoinShape2;
			break;
		}
	}

	if (m_instanceTimer > 0.2f)
	{
		m_isAlive = false;
	}

	m_posY += m_velY * GET_DELTATIME();
}

void MyGame::JumpCoin::CheckCollision()
{

}

void MyGame::JumpCoin::CallInteract(int collisionFlag, int otherFlag)
{

}
