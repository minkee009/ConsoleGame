// ConsoleGame.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "engine.hpp"
#include "input.hpp"
#include "time.hpp"

using namespace MyGame;

int main()
{
	ConsoleRenderer render(160,60);

	int x, y;

	x = 0;
	y = 0;

	SPRITE spr;

	const WCHAR* dot = L"██████████████████";
	const WORD atr = FOREGROUND_BLUE | FOREGROUND_INTENSITY;

	spr.ShapeString = dot;
	spr.Size = { 6, 3 };
	spr.Position = { 4, 2 };
	spr.Pivot = { 1, 1 };
	spr.SortingOrder = 15;
	spr.Attribute = atr;

	SPRITE spr2;

	spr2.ShapeString = dot;
	spr2.Size = { 3, 3 };
	spr2.Position = { 0, 0 };
	spr2.Pivot = { 1, 1 };
	spr2.SortingOrder = 5;
	spr2.Attribute = FOREGROUND_RED | FOREGROUND_INTENSITY;

	SPRITE spr3;

	spr3.ShapeString = L"█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█\0█";
	spr3.Size = { 15, 15 };
	spr3.Position = { 9 , 22 };
	spr3.Pivot = { 5, 6 };
	spr3.SortingOrder = 4;
	spr3.Attribute = FOREGROUND_RED | FOREGROUND_INTENSITY;


	SPRITE spr1;
	spr1.ShapeString = L"안녕하세요 이 프로그램은 한글을 지원합니다";
	spr1.Size = { 23, 1 };
	spr1.Pivot = { 0 , 0 };
	spr1.Position = { 0, 0 };//{ (SHORT)(render.GetScreenWidth() / 2 - 6), (SHORT)(render.GetScreenHeight() / 2) };
	spr1.SortingOrder = 3;
	spr1.Attribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

	INIT_TIME();

	float sx = 0.0f, sy = 0.0f;
	float speed = 12.0f;
	
	float timer = 0;
	float viewCenterX = 0, viewCenterY = 0;

	while (1)
	{
		UPDATE_TIME();
		SCAN_KEY();

		//timer += GET_DELTATIME();

		//if (timer > 1.0f)
		//{
		//	timer = 0;
		//	render.SetViewPortCenter(++viewCenterX, ++viewCenterY);
		//}

		sx += ((float)GET_KEY(VK_RIGHT) - (float)GET_KEY(VK_LEFT)) * speed * GET_DELTATIME();
		sy += ((float)GET_KEY(VK_DOWN) - (float)GET_KEY(VK_UP)) * speed * GET_DELTATIME();

		render.SetViewPortCenter(std::round(sx), std::round(sy));

		//spr.Position.X = sx;//+= (int)GET_KEY_DOWN(VK_RIGHT) - (int)GET_KEY_DOWN(VK_LEFT);
		//spr.Position.Y = sy;//+= (int)GET_KEY_DOWN(VK_DOWN) - (int)GET_KEY_DOWN(VK_UP);

		render.AddDrawCall(&spr1);
		render.AddDrawCall(&spr2);
		render.AddDrawCall(&spr3);
		render.AddDrawCall(&spr);

		render.Render();
	}

	return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
