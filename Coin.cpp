#include "Coin.hpp"
#include "engine.hpp"
#include "math.hpp"

MyGame::Coin::Coin(PlayScene* scene)
{
	m_scene = scene;
	m_spr =
	{
		{COIN_SPR_SIZE_X,COIN_SPR_SIZE_Y},
		{ 0, 0 },
		m_coinShape1,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};
}

void MyGame::Coin::Initialize()
{
	m_isAlive = true;
	m_timer = 0.0f;
	m_spr.ShapeString = m_coinShape1;
}

void MyGame::Coin::Update()
{
	m_timer += GET_DELTATIME();
	if (m_timer > 0.4f)
	{
		m_timer = 0.0f;
		m_sprIdx++;
		m_sprIdx %= COIN_SPR_ANIM_MAXFRAME;
		switch (m_sprIdx)
		{
		case 0:
			m_spr.ShapeString = m_coinShape1;
			break;
		case 1:
			m_spr.ShapeString = m_coinShape2;
			break;
		}
	}
}

void MyGame::Coin::CallInteract(int collisionFlag, int otherFlag)
{
	if (otherFlag & IS_PLAYER)
	{
		m_isAlive = false;
		m_scene->AddScore(100);
		PLAY_SE("SE\\coin.mp3", 1);
	}
}
