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
	//포인터 위치
	if (GET_KEY_DOWN(VK_UP))
		m_menuPointer--;

	if (GET_KEY_DOWN(VK_DOWN))
		m_menuPointer++;

	m_menuPointer = m_menuPointer < 0 ? 0 : m_menuPointer;
	m_menuPointer %= MENUSENE_MENUPOINTER_MAXROW;

	if (GET_KEY_DOWN(VK_RETURN))
	{
		switch (m_menuPointer)
		{
		case 0:
			CHANGE_SCENE(Play);
			break;
		case 1:
			ENGINE_EXIT();
			break;
		}
	}
}

void MenuScene::CreateDrawCall()
{
	SHORT menuPointerY = 0;

	switch (m_menuPointer)
	{
	case 0:
		menuPointerY = 3;
		break;
	case 1:
		menuPointerY = 5;
		break;
	}

	//드로우콜
	DRAW_CALL_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 5), menuPointerY }, L">");
	DRAW_CALL_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 3), 3 }, L"게임 시작");
	DRAW_CALL_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 3), 5 }, L"게임 종료");
}



