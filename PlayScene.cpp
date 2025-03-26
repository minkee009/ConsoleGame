#include "PlayScene.hpp"
#include "engine.hpp"

using MyGame::Engine;

static const WCHAR* playerShape[] = {
	L"█ ███",
	L"███ █",
	L"  ███" };
static const WORD playerShapeColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;

void PlayScene::Initialize()
{
	/*COORD Size;
	COORD Pivot;
	SHORT SortingOrder;
	const WCHAR** ShapeString;
	WORD Attribute;
	BOOL Flip;*/
	m_playerPos = { 5, 20 };
	m_playerSpr =
	{
		{ 5, 3 },
		{ 2, 2 },
		1,
		playerShape,
		playerShapeColor,
		false
	};
}

void PlayScene::Update()
{
	DRAW_CALL_SPR(m_playerPos, &m_playerSpr);
}



