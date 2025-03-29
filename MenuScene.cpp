#include "MenuScene.hpp"
#include "engine.hpp"

using MyGame::Engine;

void MyGame::MenuScene::Initialize()
{
	/*COORD Size;
	COORD Pivot;
	SHORT SortingOrder;
	const WCHAR** ShapeString;
	WORD Attribute;
	BOOL Flip;*/

	m_menuPointer = 0;

}

void MyGame::MenuScene::Update()
{
	//포인터 위치
	if (GET_KEY_DOWN(VK_UP))
		m_menuPointer--;

	if (GET_KEY_DOWN(VK_DOWN))
		m_menuPointer++;

	m_menuPointer = m_menuPointer < 0 ? MENUSCENE_MENUPOINTER_MAXROW - 1 : m_menuPointer;
	m_menuPointer %= MENUSCENE_MENUPOINTER_MAXROW;

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

void MyGame::MenuScene::Render()
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
	RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 5), menuPointerY }, L">");
	RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 3),3 }, L"게임 시작");
	RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 3), 5 }, L"게임 종료");
}



