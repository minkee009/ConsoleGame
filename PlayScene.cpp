﻿#include "PlayScene.hpp"
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

PlayScene::PlayScene()
{
	m_renderedMap = new WCHAR*[MAP01_SPR_SIZE_Y * MAP01_SIZE_Y];
	for (int i = 0; i < MAP01_SPR_SIZE_Y * MAP01_SIZE_Y; i++)
		m_renderedMap[i] = new WCHAR[MAP01_SPR_SIZE_X * MAP01_SIZE_X + 1];
}

void PlayScene::Initialize()
{
	Engine::GetInstance()->GetConsoleRenderer()->SetViewPortAnchor(0, -16);

	m_isGrounded = true;
	m_jumpTimer = 0.0f;
	m_jumpTrigger = false;
	m_playerPosX = 0;
	m_playerPosY = 0;

	m_playerStep = 0.0f;

	m_currentSprNum = 0;

	m_playerSpr =
	{
		{ PLAYER_SPR_SIZE_X, PLAYER_SPR_SIZE_Y },
		{ PLAYER_SPR_PIVOT_X, PLAYER_SPR_PIVOT_Y },
		m_playerShape1,
		m_commonShapeColor,
		false
	};

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

}

void PlayScene::Update()
{
	int playerMapXidx = (int)(m_playerPosX < 0.0f ? m_playerPosX - MAP01_SPR_SIZE_X : m_playerPosX) / MAP01_SPR_SIZE_X;
	int playerMapYidx = (int)(m_playerPosY < 0.0f ? m_playerPosY - MAP01_SPR_SIZE_Y : m_playerPosY) / MAP01_SPR_SIZE_Y;

	//바닥 체크
	//발 밑 3칸중 땅이 있는지 체크 후 발과 땅의 거리 계산
	auto lastGrounded = m_isGrounded;
	m_isGrounded = false;
	float playerFootY = m_playerPosY + PLAYER_SPR_SIZE_Y;

	if (m_player_velY >= -0.001f)
	for (int i = playerMapYidx + 1; i <= playerMapYidx + 2; i++)
		for (int j = playerMapXidx; j <= playerMapXidx + 2; j++)
		{
			if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
				continue;

			auto pickY = (i) * MAP01_SPR_SIZE_Y - 0.02f;//타일의 y좌표
			auto pick = m_map01[i][j];	 //현재 타일
			auto pickXMin = j * MAP01_SPR_SIZE_X;        //타일의 x좌표 최소값
			auto pickXMax = (j + 1) * MAP01_SPR_SIZE_X;  //타일의 x좌표 최대값

			if ((CheckTileCollision(pick))
				&& ((pickXMax > m_playerPosX && pickXMin < m_playerPosX + PLAYER_SPR_SIZE_X)
				|| (pickXMin < m_playerPosX + PLAYER_SPR_SIZE_X && pickXMax >= m_playerPosX))
				&& pickY < m_playerPosY + PLAYER_SPR_SIZE_Y)
			{
				m_isGrounded = true;
				m_player_velY = 0.0f;
				break;
			}
		}

	if (!lastGrounded && m_isGrounded)
		m_playerSpr.ShapeString = m_playerShape1;

	//속도 계산
	auto hInput = (GET_KEY(VK_RIGHT) ? 1 : 0) - (GET_KEY(VK_LEFT) ? 1 : 0);
	auto vInput = (GET_KEY(VK_DOWN) ? 1 : 0) - (GET_KEY(VK_UP) ? 1 : 0);

	if (m_isGrounded)
	{
		if (m_playerStep > PLAYER_STEPRATE)
		{
			m_currentSprNum++;
			m_currentSprNum %= PLAYER_SPR_ANIM_MAXFRAME;

			switch (m_currentSprNum)
			{
			case 0:
				m_playerSpr.ShapeString = m_playerShape1;
				break;
			case 1:
				m_playerSpr.ShapeString = m_playerShape2;
				break;
			}

			m_playerStep = 0.0f;
		}
		if (hInput != 0)
		{
			m_playerStep += GET_DELTATIME();
			m_playerSpr.Flip = hInput > 0 ? false : true;
			m_player_velX += hInput * PLAYER_ACCEL * ((hInput * m_player_velX < 0) ? 2.0f : 1.0f) * (GET_KEY(VK_LSHIFT) ? 2.0f : 1.0f) * GET_DELTATIME();
			m_player_velX = max(-PLAYER_MAXSPEED * (GET_KEY(VK_LSHIFT) ? 2.0f : 1.0f), min(PLAYER_MAXSPEED * (GET_KEY(VK_LSHIFT) ? 2.0f : 1.0f), m_player_velX));
		}
		else
		{
			m_player_velX = m_player_velX > 0 ? max(0.0f, m_player_velX - PLAYER_DECEL * GET_DELTATIME()) : min(0.0f, m_player_velX + PLAYER_DECEL * GET_DELTATIME());
		}

		if (GET_KEY_DOWN(VK_SPACE))
		{
			m_jumpTrigger = true;
			m_player_velY = -PLAYER_JUMPVEL;
			m_playerSpr.ShapeString = m_playerShape3;
		}
	}
	else 
	{
		m_player_velX += hInput * PLAYER_AIR_ACCEL * GET_DELTATIME();
		if (m_jumpTrigger)
		{
			m_jumpTimer += GET_DELTATIME();
			if (m_jumpTimer > PLAYER_JUMPTIME || GET_KEY_UP(VK_SPACE))
			{
				m_jumpTrigger = false;
				m_jumpTimer = 0.0f;
			}
		}
		else
		{
			m_player_velY += PLAYER_GRAVITY * GET_DELTATIME();
			m_player_velY = min(PLAYER_MAXFALLSPEED, m_player_velY);
		}
	}

		
	m_playerPosX += m_player_velX * GET_DELTATIME();
	m_playerPosY += m_player_velY * GET_DELTATIME();


	//맵 충돌 체크
	playerMapXidx = (int)(m_playerPosX < 0.0f ? m_playerPosX - MAP01_SPR_SIZE_X : m_playerPosX) / MAP01_SPR_SIZE_X;
	playerMapYidx = (int)(m_playerPosY < 0.0f ? m_playerPosY - MAP01_SPR_SIZE_Y : m_playerPosY) / MAP01_SPR_SIZE_Y;

	float debugA = m_playerPosY;
	float debugB = 0.0f;

	for (int i = playerMapYidx; i <= playerMapYidx + 2; i++)
		for (int j = playerMapXidx; j <= playerMapXidx + 2; j++)
		{
			if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
				continue;

			if (CheckTileCollision(m_map01[i][j]) &&
				!((m_playerPosX + PLAYER_SPR_SIZE_X < j * MAP01_SPR_SIZE_X)
					|| (m_playerPosX > (j + 1) * MAP01_SPR_SIZE_X)
					|| (m_playerPosY + PLAYER_SPR_SIZE_Y < i * MAP01_SPR_SIZE_Y)
					|| (m_playerPosY > (i + 1) * MAP01_SPR_SIZE_Y)))
			{

				//침투 계산
				float penetrationX = min(m_playerPosX + PLAYER_SPR_SIZE_X - j * MAP01_SPR_SIZE_X, (j + 1) * MAP01_SPR_SIZE_X - m_playerPosX);
				float penetrationY = min(m_playerPosY + PLAYER_SPR_SIZE_Y - i * MAP01_SPR_SIZE_Y, (i + 1) * MAP01_SPR_SIZE_Y - m_playerPosY);

				// X축과 Y축 중 더 작은 침투 방향으로 이동하여 충돌 해결
				if (penetrationX < penetrationY) {
					if (m_playerPosX < j * MAP01_SPR_SIZE_X)
					{
						auto prevPosX = m_playerPosX;
						m_playerPosX -= penetrationX; // 왼쪽으로 밀기
						if (abs(m_playerPosX - prevPosX) >= 0.000001)
							m_playerPosX = round(m_playerPosX);
					}
					else
						m_playerPosX += penetrationX; // 오른쪽으로 밀기

					if (m_playerPosY + PLAYER_SPR_SIZE_Y - 0.5 >= i * MAP01_SPR_SIZE_Y
						&& m_playerPosY + 0.5 < (i + 1) * MAP01_SPR_SIZE_Y)
						m_player_velX = 0.0f;

				}
				else {
					if (m_playerPosY < i * MAP01_SPR_SIZE_Y)
					{
						m_playerPosY = i * MAP01_SPR_SIZE_Y - PLAYER_SPR_SIZE_Y;// 위로 밀기
					}
					else
					{
						m_playerPosY += penetrationY; // 아래로 밀기
						m_jumpTrigger = false;
						m_jumpTimer = 0.0f;
						m_player_velY = 0.0f;
					}
				}					
				
				debugA = m_playerPosY + 0.5;
				debugB = (i + 1) * MAP01_SPR_SIZE_Y;
			}
		}

	//DebugPos = { (SHORT)playerMapXidx * MAP01_SPR_SIZE_X,(short)playerMapYidx * MAP01_SPR_SIZE_Y };
	//swprintf_s(DebugMsg, 1024, L"█ <- { %f , %f} ", debugA, debugB);

	//뷰포트 이동

	//플레이어가 화면 중앙을 넘어갈 경우 뷰포트 이동
	if ((SHORT)m_playerPosX + (m_playerSpr.Size.X >> 1) - GET_ANCHOR_POS().X > (GET_SCREEN_WIDTH() >> 1))
	{
		SET_ANCHOR_POS((SHORT)m_playerPosX + (m_playerSpr.Size.X >> 1) - (GET_SCREEN_WIDTH() >> 1), GET_ANCHOR_POS().Y);
	}

	//맵의 끝이 화면 오른쪽에 있는 경우 뷰포트 제한
	if (MAP01_SIZE_X * MAP01_SPR_SIZE_X - GET_ANCHOR_POS().X < GET_SCREEN_WIDTH())
	{
		SET_ANCHOR_POS(MAP01_SIZE_X * MAP01_SPR_SIZE_X - GET_SCREEN_WIDTH(), GET_ANCHOR_POS().Y);
	}

	//뷰포트 왼쪽 벽 처리
	if ((SHORT)m_playerPosX < GET_ANCHOR_POS().X)
	{
		m_playerPosX = GET_ANCHOR_POS().X;
	}

	//뷰포트 오른쪽 벽 처리
	if ((SHORT)m_playerPosX + PLAYER_SPR_SIZE_X > GET_ANCHOR_POS().X + GET_SCREEN_WIDTH() - 1)
	{
		m_playerPosX = GET_ANCHOR_POS().X + GET_SCREEN_WIDTH() - PLAYER_SPR_SIZE_X - 1;
	}
}


void PlayScene::Render()
{
	for (int i = 0; i < MAP01_SPR_SIZE_Y * MAP01_SIZE_Y; i++)
	{
		COORD pos = { 0, (short)i };
		const wchar_t* d = m_renderedMap[i];

		Engine::GetInstance()->GetConsoleRenderer()->WStringDraw(pos, d, MAP01_SIZE_X * MAP01_SPR_SIZE_X);
	}


	RENDER_SPR({ (SHORT)(ceil(m_playerPosX)), (SHORT)(ceil(m_playerPosY)) }, &m_playerSpr);



	//swprintf_s(DebugMsg, 1024, L"{ %f } ",1 / GET_DELTATIME());
	//RENDER_STR(DebugPos, DebugMsg);
}
	
	

PlayScene::~PlayScene()
{
	if (m_renderedMap)
		delete[] m_renderedMap;
}


