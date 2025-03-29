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

using namespace MyGame;

//constexpr float PI_F = 3.14159265358979323846f;
//constexpr float DEG_TO_RAD = 3.1415927f / 180.0f;
//
//float lerp_f(float a, float b, float t)
//{
//	t = t > 1.0f ? 1.0f : (t < 0 ? 0.0f : t);
//	return a + (b - a) * t;
//}
//
//float sign_f(float num)
//{
//	return num > 0 ? 1.0f : -1.0f;
//}

int main()
{
	Engine::Initialize(120, 40);

	MenuScene* menuScene = new MenuScene;
	PlayScene* playScene = new PlayScene;
	SetRankScene* setRankScene = new SetRankScene;

	ADD_SCENE(menuScene);
	ADD_SCENE(playScene);
	ADD_SCENE(setRankScene);

	Engine::GetInstance()->Run();

	delete playScene;
	delete menuScene;
	delete setRankScene;

	return 0;
}