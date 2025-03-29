#include "GoalFlag.hpp"
#include "engine.hpp"
#include "Player.hpp"
#include "GoalPole.hpp"

MyGame::GoalFlag::GoalFlag(PlayScene* scene)
{
	m_scene = scene;

	m_spr =
	{
		{ GOAL_FLAG_SPR_SIZE_X, GOAL_FLAG_SPR_SIZE_Y },
		{ 0, 0 },
		m_goalFlagShape,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};
}

void MyGame::GoalFlag::Initialize()
{
	m_posY = m_spawnPosY;
}

void MyGame::GoalFlag::Update()
{
	if (m_scene->gameState == Goal)
	{
		m_posY += GET_DELTATIME() * (PLAYER_MAXFALLSPEED >> 1);
		m_posY = min(m_spawnPosY + GOAL_POLE_SPR_SIZE_Y - GOAL_FLAG_SPR_SIZE_Y, m_posY);
	}
}
