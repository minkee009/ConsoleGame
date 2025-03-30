#include "GameOverScene.hpp"
#include "engine.hpp"

void MyGame::GameOverScene::Initialize()
{
	m_timer = 0.0f;
	SET_ANCHOR_POS(0, 0);
}

void MyGame::GameOverScene::Update()
{
	m_timer += GET_DELTATIME();

	if (m_timer > 3.0f)
	{
		CHANGE_SCENE(Menu);
	}
}

void MyGame::GameOverScene::Render()
{
	RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 2),(SHORT)((GET_SCREEN_HEIGHT() >> 1)) }, L"���ӿ���");
}
