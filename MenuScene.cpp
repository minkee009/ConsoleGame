#include "MenuScene.hpp"
#include "engine.hpp"

using MyGame::Engine;

void MenuScene::Initialize()
{
	/*COORD Size;
	COORD Pivot;
	SHORT SortingOrder;
	const WCHAR** ShapeString;
	WORD Attribute;
	BOOL Flip;*/

	m_menuPointer = 0;

}

void MenuScene::Update()
{
	DRAW_CALL_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 3), 3}, L"게임 시작");
	DRAW_CALL_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 3), 5}, L"게임 시작");
}



