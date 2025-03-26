// ConsoleGame.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <cmath>
#include "engine.hpp"
#include "input.hpp"
#include "time.hpp"

using namespace MyGame;

constexpr float PI_F = 3.14159265358979323846f;

float lerp_f(float a, float b, float t)
{
	return a + (b - a) * t;
}

float sign_f(float num)
{
	return num > 0 ? 1.0f : -1.0f;
}

int main()
{
	ConsoleRenderer render(160,60);

	int x, y;

	x = 0;
	y = 0;

	SPRITE spr;

	const WCHAR* dot[] = { L"█ ███",
						   L"███ █",
						   L"  ███" };
	const WORD atr = FOREGROUND_BLUE | FOREGROUND_INTENSITY;

	spr.ShapeString = dot;
	spr.Size = { 5, 3 };
	spr.Pivot = { 2, 1 };
	spr.SortingOrder = 1;
	spr.Attribute = atr;
	spr.Flip = true;

	SPRITE spr2;

	spr2.ShapeString = dot;
	spr2.Size = { 3, 3 };
	spr2.Pivot = { 1, 1 };
	spr2.SortingOrder = 5;
	spr2.Attribute = FOREGROUND_RED | FOREGROUND_INTENSITY;
	spr2.Flip = false;

	SPRITE spr3;

	const WCHAR* dot2[] = {
		L"█ █ █ █ █ █ █ █",
		L" █ █ █ █ █ █ █ ",
		L"█ █ █ █ █ █ █ █",
		L" █ █ █ █ █ █ █ ",
		L"█ █ █ █ █ █ █ █",
		L" █ █ █ █ █ █ █ ",
		L"█ █ █ █ █ █ █ █",
		L" █ █ █ █ █ █ █ ",
		L"█ █ █ █ █ █ █ █",
		L" █ █ █ █ █ █ █ ",
		L"█ █ █ █ █ █ █ █",
		L" █ █ █ █ █ █ █ ",
		L"█ █ █ █ █ █ █ █",
        L" █ █ █ █ █ █ █ ",
		L"█ █ █ █ █ █ █ █",
	};

	spr3.ShapeString = dot2;
	spr3.Size = { 15 , 15 };
	spr3.Pivot = { 5, 6 };
	spr3.SortingOrder = 4;
	spr3.Attribute = FOREGROUND_RED | FOREGROUND_INTENSITY;
	spr3.Flip = false;

	INIT_TIME();

	float sx = 0.0f, sy = 0.0f;
	float speed = 12.0f;
	
	float timer = 0;
	float viewCenterX = 0, viewCenterY = 0;

	//Engine::Initialize(160, 60);

	//Engine::GetInstance()->Run();

	COORD sprPos;
	sprPos.X = (SHORT)(render.GetScreenWidth() / 2);
	sprPos.Y = (SHORT)(render.GetScreenHeight() / 2);

	float plyX = 0, plyY = 0;
	plyX = sprPos.X;
	plyY = sprPos.Y;

	while (1)
	{
		UPDATE_TIME();
		SCAN_KEY();

		auto hInput = ((float)GET_KEY(VK_RIGHT) - (float)GET_KEY(VK_LEFT)) * speed * GET_DELTATIME();
		auto vInput = ((float)GET_KEY(VK_DOWN) - (float)GET_KEY(VK_UP)) * speed * GET_DELTATIME();
		
		if (GET_KEY_DOWN(VK_SPACE))
		{
			hInput += hInput != 0 ? sign_f(hInput) * 20 : 0;
			vInput += vInput != 0 ? sign_f(vInput) * 10 : 0;
		}
		
		if(hInput != 0)
			spr.Flip = hInput > 0 ? true : false;
		

		auto center = render.GetViewPortAnchor();

		plyX += hInput;
		plyY += vInput;

		sprPos.X = std::round(plyX);
		sprPos.Y = std::round(plyY);

		float fourX = plyX + sinf(GET_TOTALTIME() * PI_F * 2.0f) * 8.0f;
		float fourY = plyY + cosf(GET_TOTALTIME() * PI_F * 2.0f) * 4.0f;

		render.AddStringDrawCall({ 43, 32 }, L"안녕하세요 이 프로그램은 한글을 지원합니다");
		render.AddSpriteDrawCall({ (SHORT)std::round(fourX) , (SHORT)std::round(fourY) }, &spr2);
		render.AddSpriteDrawCall({ 9 , 22 }, &spr3);
		render.AddSpriteDrawCall(sprPos, &spr);

		render.Render();

		float targetX = plyX - (SHORT)(render.GetScreenWidth() / 2);
		float targetY = plyY - (SHORT)(render.GetScreenHeight() / 2);

		sx = lerp_f(sx, targetX, 3.2f * GET_DELTATIME());
		sy = lerp_f(sy, targetY, 3.2f * GET_DELTATIME());

		render.SetViewPortCenter(std::round(sx), std::round(sy));

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
