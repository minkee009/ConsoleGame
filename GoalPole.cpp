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

MyGame::GoalPole::~GoalPole()
{
}

void MyGame::GoalPole::Update()
{

}

void MyGame::GoalPole::CallInteract(int collisionFlag)
{
	m_scene->gameState = Goal;
}
