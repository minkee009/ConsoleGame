#include "PlayScene.hpp"
#include "engine.hpp"
#include <cwchar>

using MyGame::Engine;

wchar_t DebugMsg[1024];
wchar_t DebugMsg1[1024];
wchar_t DebugMsg2[1024];
wchar_t DebugMsg3[1024];

COORD DebugPos = { 0, 0 };
COORD DebugPos1 = { 0, 0 };
COORD DebugPos2 = { 0, 0 };
COORD DebugPos3 = { 0, 0 };

MyGame::PlayScene::PlayScene()
{
	m_renderedMap = new WCHAR*[MAP01_SPR_SIZE_Y * MAP01_SIZE_Y];
	for (int i = 0; i < MAP01_SPR_SIZE_Y * MAP01_SIZE_Y; i++)
		m_renderedMap[i] = new WCHAR[MAP01_SPR_SIZE_X * MAP01_SIZE_X + 1];

	m_map01_groundSpr =
	{
		{ MAP01_SPR_SIZE_X, MAP01_SPR_SIZE_Y },
		{ 0, 0 },
		m_map01_gShape,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};

	m_map01_dgroundSpr =
	{
		{ MAP01_SPR_SIZE_X, MAP01_SPR_SIZE_Y },
		{ 0, 0 },
		m_map01_dShape,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};

	m_map01_pipe0Spr =
	{
		{ MAP01_SPR_SIZE_X, MAP01_SPR_SIZE_Y },
		{ 0, 0 },
		m_map01_pipe0Shape,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};

	m_map01_pipe1Spr =
	{
		{ MAP01_SPR_SIZE_X, MAP01_SPR_SIZE_Y },
		{ 0, 0 },
		m_map01_pipe1Shape,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};

	m_map01_pipe2Spr =
	{
		{ MAP01_SPR_SIZE_X, MAP01_SPR_SIZE_Y },
		{ 0, 0 },
		m_map01_pipe2Shape,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};

	m_map01_pipe3Spr =
	{
		{ MAP01_SPR_SIZE_X, MAP01_SPR_SIZE_Y },
		{ 0, 0 },
		m_map01_pipe3Shape,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};

	m_map01_solidBlockSpr =
	{
		{ MAP01_SPR_SIZE_X, MAP01_SPR_SIZE_Y },
		{ 0, 0 },
		m_map01_solidBlock,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};

	// 사전 렌더링
	for (int i = 0; i < MAP01_SIZE_Y; i++) {
		for (int j = 0; j < MAP01_SIZE_X; j++) {
			switch (m_map01[i][j]) {
			case 'g':
				for (int k = 0; k < MAP01_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * MAP01_SPR_SIZE_Y + k][j * MAP01_SPR_SIZE_X], m_map01_groundSpr.ShapeString[k], MAP01_SPR_SIZE_X);
				}
				break;
			case 'd':
				for (int k = 0; k < MAP01_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * MAP01_SPR_SIZE_Y + k][j * MAP01_SPR_SIZE_X], m_map01_dgroundSpr.ShapeString[k], MAP01_SPR_SIZE_X);
				}
				break;
			case 'y':
				for (int k = 0; k < MAP01_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * MAP01_SPR_SIZE_Y + k][j * MAP01_SPR_SIZE_X], m_map01_pipe0Spr.ShapeString[k], MAP01_SPR_SIZE_X);
				}
				break;
			case 'u':
				for (int k = 0; k < MAP01_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * MAP01_SPR_SIZE_Y + k][j * MAP01_SPR_SIZE_X], m_map01_pipe1Spr.ShapeString[k], MAP01_SPR_SIZE_X);
				}
				break;
			case 'j':
				for (int k = 0; k < MAP01_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * MAP01_SPR_SIZE_Y + k][j * MAP01_SPR_SIZE_X], m_map01_pipe2Spr.ShapeString[k], MAP01_SPR_SIZE_X);
				}
				break;
			case 'k':
				for (int k = 0; k < MAP01_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * MAP01_SPR_SIZE_Y + k][j * MAP01_SPR_SIZE_X], m_map01_pipe3Spr.ShapeString[k], MAP01_SPR_SIZE_X);
				}
				break;
			case 's':
				for (int k = 0; k < MAP01_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * MAP01_SPR_SIZE_Y + k][j * MAP01_SPR_SIZE_X], m_map01_solidBlockSpr.ShapeString[k], MAP01_SPR_SIZE_X);
				}
				break;
			default:
				for (int k = 0; k < MAP01_SPR_SIZE_Y; k++) {
					wmemset(&m_renderedMap[i * MAP01_SPR_SIZE_Y + k][j * MAP01_SPR_SIZE_X], L' ', MAP01_SPR_SIZE_X);
				}
				break;
			}
		}
	}

	m_player = new MyGame::Player(this);
}

void MyGame::PlayScene::Initialize()
{
	Engine::GetInstance()->GetConsoleRenderer()->SetViewPortAnchor(0, -16);

}

void MyGame::PlayScene::Update()
{
	m_player->UpdateMovement();
	m_player->UpdateCollision();
}


void MyGame::PlayScene::Render()
{
	for (int i = 0; i < MAP01_SPR_SIZE_Y * MAP01_SIZE_Y; i++)
	{
		COORD pos = { 0, (short)i };
		const wchar_t* d = m_renderedMap[i];

		Engine::GetInstance()->GetConsoleRenderer()->WStringDraw(pos, d, MAP01_SIZE_X * MAP01_SPR_SIZE_X);
	}


	RENDER_SPR({ (SHORT)(ceil(m_player->GetPosX())), (SHORT)(ceil(m_player->GetPosY())) }, m_player->GetSprite());



	//swprintf_s(DebugMsg, 1024, L"{ %f } ",1 / GET_DELTATIME());
	//RENDER_STR(DebugPos, DebugMsg);
}
	
	

MyGame::PlayScene::~PlayScene()
{
	if (m_renderedMap)
		delete[] m_renderedMap;

	delete m_player;
}


