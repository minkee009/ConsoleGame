// ConsoleGame.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

//std
#include <iostream>
#include <cmath>

//engine
#include "engine.hpp"
#include "input.hpp"
#include "time.hpp"

//Scene
#include "MenuScene.hpp"
#include "PlayScene.hpp"
#include "SetRankScene.hpp"
#include "ViewRankScene.hpp"
#include "GameOverScene.hpp"

using namespace MyGame;

int main()
{
	Engine::Initialize(120, 40);

	MenuScene* menuScene = new MenuScene;
	PlayScene* playScene = new PlayScene;
	SetRankScene* setRankScene = new SetRankScene;
	ViewRankScene* viewRankScene = new ViewRankScene;
	GameOverScene* gameOverScene = new GameOverScene;

	ADD_SCENE(menuScene);
	ADD_SCENE(playScene);
	ADD_SCENE(setRankScene);
	ADD_SCENE(viewRankScene);
	ADD_SCENE(gameOverScene);

	Engine::GetInstance()->Run();

	delete playScene;
	delete menuScene;
	delete setRankScene;
	delete viewRankScene;
	delete gameOverScene;

	return 0;
}