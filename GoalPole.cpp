#include "GoalPole.hpp"

MyGame::GoalPole::GoalPole(PlayScene* scene)
{
	Initialize();
	m_scene = scene;
	m_spr =
	{
		{ GOAL_POLE_SPR_SIZE_X, GOAL_POLE_SPR_SIZE_Y },
		{ 0, 0 },
		m_goalPoleShape,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};
}

void MyGame::GoalPole::CallInteract(int collisionFlag)
{
	//게임상태 -> 골
	m_scene->gameState = Goal;
	m_scene->GetPlayer()->SetPosition(m_posX - PLAYER_SPR_SIZE_X, m_scene->GetPlayer()->GetPosY());

	//높이에 따라 점수 주기
	if (m_scene->GetPlayer()->GetPosY() - m_posY < 0)
	{
		m_scene->AddScore(5000);
	}
	else if (m_scene->GetPlayer()->GetPosY() - m_posY < 12)
	{
		m_scene->AddScore(2000);
	}
	else if (m_scene->GetPlayer()->GetPosY() - m_posY < 22)
	{
		m_scene->AddScore(1000);
	}
	else 
	{
		m_scene->AddScore(500);
	}
}
