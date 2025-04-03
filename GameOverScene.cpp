#include "GameOverScene.hpp"
#include "engine.hpp"

void MyGame::GameOverScene::Initialize()
{
	m_timer = 0.0f;
	SET_ANCHOR_POS(0, 0);
	PLAY_SE("SE\\gameover.mp3", 2);
}

void MyGame::GameOverScene::Update()
{
	m_timer += GET_DELTATIME();

	if (m_timer > 4.5f)
	{
		CHANGE_SCENE(Menu);
	}
}

void MyGame::GameOverScene::Render()
{
	RENDER_STR({ 36,15 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣴⣴⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,16 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⣼⡟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,17 }, L"⠀⠀⠀⠀⠀⠀⠀⢰⡿⠀⠀⣀⣀⡀⠀⠀⢀⣤⣄⠀⠀⢀⣀⣀⡀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,18 }, L"⠀⠀⠀⠀⠀⠀⠀⠸⣧⡀⠀⠉⢹⡇⠀⢰⣟⣁⣿⡇⠀⣾⡿⢻⣷⠿⣷⣰⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,19 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠙⠛⠿⠾⠟⠃⠀⠈⠛⠋⠙⠛⠀⠛⠀⠈⠏⠈⠛⢿⣯⣯⡶⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,20 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣌⠻⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,21 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⡾⠛⠋⠁⠀⣻⡇⠀⠀⠀⠀⠀⣀⠀⣀⣀⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,22 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⡿⠀⠀⠀⠀⢀⣿⠂⠀⢻⣆⢀⣾⢟⣾⣻⣙⣻⣗⠀⣾⣤⡶⠖⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,23 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣆⣀⣀⣀⣼⠏⠀⠀⠈⢻⡿⠋⢸⣇⣉⢉⣙⠀⠀⢸⣿⠁⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,24 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠋⠋⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠙⠛⠛⠋⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀");
}
