// ConsoleGame.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "engine.hpp"
#include "input.hpp"

using namespace MyGame;

int main()
{
	ConsoleRenderer render(120,48);

	int x, y;

	x = 0;
	y = 0;

	SPRITE spr;

	CHAR_INFO pixels[9];

	spr.Pixels = pixels;
	spr.Size = { 3, 3 };
	spr.Position = { 0, 0 };
	spr.Pivot = { 1, 1 };
	spr.SortingOrder = 5;

	for (int i = 0; i < 9; i++) {
		pixels[i].Char.UnicodeChar = L'█'; // 유니코드 블록 문자
		pixels[i].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY; // 밝은 초록색
	}


	pixels[0].Char.UnicodeChar = 0;

	pixels[4].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

	SPRITE spr1;

	CHAR_INFO pixels1[13];

	spr1.Pixels = pixels1;
	spr1.Size = { 13, 1 };
	spr1.Pivot = { 0 , 0 };
	spr1.Position = { (SHORT)(render.GetScreenWidth() / 2 - 6), (SHORT)(render.GetScreenHeight() / 2) };
	spr1.SortingOrder = 3;

	std::wstring hello = L"hello, world!";

	for (int i = 0; i < hello.size(); i++) {
		pixels1[i].Char.UnicodeChar = hello[i]; // 유니코드 블록 문자
		pixels1[i].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY; // 밝은 초록색
	}


	while (1)
	{
		SCAN_KEY();
		spr.Position.X += (int)GET_KEY_DOWN(VK_RIGHT) - (int)GET_KEY_DOWN(VK_LEFT);
		spr.Position.Y += (int)GET_KEY_DOWN(VK_DOWN) - (int)GET_KEY_DOWN(VK_UP);

		render.AddDrawCall(&spr1);
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
