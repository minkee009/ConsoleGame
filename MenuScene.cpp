#include "MenuScene.hpp"
#include "engine.hpp"
#include "PlayScene.hpp"

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

	if (GET_KEY_DOWN(VK_RETURN) || GET_KEY_DOWN(VK_SPACE))
	{
		switch (m_menuPointer)
		{
		case 0:
			//플레이
			dynamic_cast<PlayScene*>(GET_SCENE(Play))->SetChildMode(false);
			CHANGE_SCENE(Play);
			break;
		case 1:
			//어린이 모드
			dynamic_cast<PlayScene*>(GET_SCENE(Play))->SetChildMode(true);
			CHANGE_SCENE(Play);
			break;
		case 2:
			CHANGE_SCENE(ViewRank);
			break;
		case 3:
			ENGINE_EXIT();
			break;
		}
	}
}

void MyGame::MenuScene::Render()
{
	SHORT menuPointerY = 24 + 2 * m_menuPointer;

	//드로우콜
	RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 7), menuPointerY }, L">");
	RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 5),24 }, L"게임 시작         ");
	RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 5),26 }, L"체험 모드              ");
	RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 5),28 }, L"명예의 전당       ");
	RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 5),30 }, L"게임 종료        ");

	RENDER_STR({ 36,7 }, L"⠀⠀⠀⠀⣀⢀⠀⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,8 }, L"⠀⠀⠀⠈⠛⠛⠛⠛⢻⡆⠀⠀⢀⣴⠿⢶⣄⠀⣿⣦⣴⡤⠀⣠⣤⣤⡀⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,9 }, L"⠀⠀⠀⠀⠀⢴⡆⠀⢸⡇⠀⠠⣿⠃⠀⠀⣿⡃⢽⣧⣤⠀⢸⣟⠀⢹⡯⠀⣾⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,10 }, L"⠀⠀⢶⢶⣶⣼⣷⣀⠈⠀⠀⠈⠿⣶⣴⣶⣟⣅⣉⠃⠉⠀⠈⠻⠾⠟⠁⢀⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,11 }, L"⠀⠀⠀⠀⠀⠉⠉⠛⠷⡦⠀⠀⠀⠀⢠⣾⠋⠋⣻⡇⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,12 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⡷⣦⡾⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,13 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡄⠀⠀⠀⠀⠀⢀⣤⡄⠀⢸⡇⠀⠀⠀⣠⣤⣄⡀⠀⠀⠀⠀");
	RENDER_STR({ 36,14 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣤⣤⡄⠀⢸⡇⠀⠀⠀⠀⠐⠟⣩⣿⠀⢸⡇⠀⠀⢸⡟⠈⠙⣷⡄⠀⠀⠀");
	RENDER_STR({ 36,15 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⠍⡁⡈⣸⡇⠀⣾⡷⡷⠾⠂⠀⢀⣾⣛⣡⡤⢸⡇⠀⠀⠈⠿⣶⣦⡿⠂⠀⠀⠀");
	RENDER_STR({ 36,16 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠈⠛⠛⠋⠁⠀⣾⠇⠀⠀⠀⠀⠀⠙⠋⠋⠁⠈⠁⠀⢰⣤⣴⡟⠁⠀⠀⠀⠀⠀");
	RENDER_STR({ 36,17 }, L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⢷⡦⠀⠀⠀⠀");
}



