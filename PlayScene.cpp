#include "PlayScene.hpp"
#include "engine.hpp"
#include "GoalPole.hpp"
#include "GoalFlag.hpp"
#include "SetRankScene.hpp"
#include <cwchar>
#include <algorithm> 

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
	m_msgBuffer = new wchar_t[MSG_BUFFER_SIZE];
	m_renderedMap = new WCHAR * [TILE_SPR_SIZE_Y * MAP01_SIZE_Y];
	for (int i = 0; i < TILE_SPR_SIZE_Y * MAP01_SIZE_Y; i++) {
		m_renderedMap[i] = new WCHAR[TILE_SPR_SIZE_X * MAP01_SIZE_X + 1];

		std::fill_n(m_renderedMap[i], TILE_SPR_SIZE_X * MAP01_SIZE_X, L' ');

		m_renderedMap[i][TILE_SPR_SIZE_X * MAP01_SIZE_X] = L'\0';
	}

	// 사전 렌더링 및 오브젝트 배치
	for (int i = 0; i < MAP01_SIZE_Y; i++) {
		for (int j = 0; j < MAP01_SIZE_X; j++) {
			switch (m_map01[i][j]) {
			case 'g':
				for (int k = 0; k < TILE_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * TILE_SPR_SIZE_Y + k][j * TILE_SPR_SIZE_X], m_map01_gShape[k], TILE_SPR_SIZE_X);
				}
				break;
			case 'd':
				for (int k = 0; k < TILE_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * TILE_SPR_SIZE_Y + k][j * TILE_SPR_SIZE_X], m_map01_dShape[k], TILE_SPR_SIZE_X);
				}
				break;
			case 'y':
				for (int k = 0; k < TILE_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * TILE_SPR_SIZE_Y + k][j * TILE_SPR_SIZE_X], m_map01_pipe0Shape[k], TILE_SPR_SIZE_X);
				}
				break;
			case 'u':
				for (int k = 0; k < TILE_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * TILE_SPR_SIZE_Y + k][j * TILE_SPR_SIZE_X], m_map01_pipe1Shape[k], TILE_SPR_SIZE_X);
				}
				break;
			case 'j':
				for (int k = 0; k < TILE_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * TILE_SPR_SIZE_Y + k][j * TILE_SPR_SIZE_X], m_map01_pipe2Shape[k], TILE_SPR_SIZE_X);
				}
				break;
			case 'k':
				for (int k = 0; k < TILE_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * TILE_SPR_SIZE_Y + k][j * TILE_SPR_SIZE_X], m_map01_pipe3Shape[k], TILE_SPR_SIZE_X);
				}
				break;
			case 's':
				for (int k = 0; k < TILE_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * TILE_SPR_SIZE_Y + k][j * TILE_SPR_SIZE_X], m_map01_solidBlock[k], TILE_SPR_SIZE_X);
				}
				break;
			case 'c':
				for (int k = 0; k < TILE_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * TILE_SPR_SIZE_Y + k][j * TILE_SPR_SIZE_X], m_map01_cloud0[k], TILE_SPR_SIZE_X);
				}
				break;
			case 'v':
				for (int k = 0; k < TILE_SPR_SIZE_Y; k++) {
					wmemcpy(&m_renderedMap[i * TILE_SPR_SIZE_Y + k][j * TILE_SPR_SIZE_X], m_map01_cloud1[k], TILE_SPR_SIZE_X);
				}
				break;
			case 'i':
			{
				//깃발, 깃대
				auto goalPole = new GoalPole(this);
				auto spawnPosX = j * TILE_SPR_SIZE_X + 2;
				auto spawnPosY = i * TILE_SPR_SIZE_Y + CorrectPosY(goalPole->GetSprite()->Size.Y);
				goalPole->setSpawnPos(spawnPosX, spawnPosY);
				goalPole->SetPosition(spawnPosX, spawnPosY);
				goalPole->Initialize();


				m_tiles.push_back({ goalPole, false });

				auto goalFlag = new GoalFlag(this);
				spawnPosX += 2;
				goalFlag->setSpawnPos(spawnPosX, spawnPosY);
				goalFlag->SetPosition(spawnPosX, spawnPosY);
				goalFlag->Initialize();

				m_tiles.push_back({ goalFlag, false });
			}
			break;
			}
		}
	}

	m_player = new MyGame::Player(this);
}

void MyGame::PlayScene::Initialize()
{
	Engine::GetInstance()->GetConsoleRenderer()->SetViewPortAnchor(0, -10);
	for (auto tile = m_tiles.begin(); tile != m_tiles.end(); ) {
		if (tile->second) {
			//인스턴스인 경우 삭제
			delete tile->first;  // 메모리 해제
			tile = m_tiles.erase(tile);  // 원소 삭제 후 반복문 인덱스 조정
		}
		else {
			tile->first->Initialize();
			++tile;
		}
	}
	m_goalTimer = 0;
	m_goalIncount = 0;
	m_deadIncount = 0;
	m_deadTimer = 0;
	m_score = 0.0f;
	m_player->Initialize();
	m_player->SetPosition(2, 26);

	m_timer = 0;

	gameState = PrintLife;
}

void MyGame::PlayScene::Update()
{
	if (m_life == 0)
	{
		//게임 오버
	}

	switch (gameState)
	{
	case PrintLife:
	{
		m_timer += GET_DELTATIME();
		if (m_timer > 2.0f)
		{
			gameState = Playing;
			m_timer = 400.0f;
		}
		break;
	}
	case Playing:
	{
		m_timer -= GET_DELTATIME() * 2.0f;
		//각 오브젝트 배열을 돌면서 m_active관리와 업데이트 관리를 시도

		if (m_timer < 0.0f)
		{
			gameState = PlayerDead;
			return;
		}

		//플레이어 움직임을 먼저 업데이트
		m_player->UpdateMovement();
		m_player->MoveViewport();
		m_player->ClampPosToViewport();

		//오브젝트 m_active관리

		//타일 관리
		for (auto tile : m_tiles)
		{
			//타일이 화면안에 들어오는지 체크
			if (tile.first->GetPosX() + tile.first->GetSprite()->Size.X < Engine::GetInstance()->GetConsoleRenderer()->GetViewPortAnchor().X)
			{
				tile.first->SetActive(false);
			}
			else
			{
				tile.first->SetActive(true);
			}
		}

		//오브젝트 업데이트


		//충돌체크
		m_player->CheckCollision();

		break;
	}
	case PlayerDead:
	{
		if (m_deadIncount == 0)
		{
			m_player->SetVelocity(0, 0);
			m_player->ForceChangePlayerShape(PLAYER_SPR_S_DIE);
			m_deadTimer += GET_DELTATIME();

			if (m_deadTimer > PLAY_DEADINCOUNT_00_TIMER)
			{
				m_deadTimer = 0.0f;
				m_deadIncount++;
			}
		}
		if (m_deadIncount == 1)
		{
			m_deadTimer += GET_DELTATIME();
			auto currentXpos = m_player->GetPosX();
			auto currentYpos = m_player->GetPosY();
			auto currentYvel = m_player->GetVelY();
			if (m_deadTimer < 0.15f)
			{
				m_player->SetVelocity(0, currentYvel -= 480.0f * GET_DELTATIME());
			}
			currentYvel += PLAYER_GRAVITY * GET_DELTATIME();
			currentYvel = min(PLAYER_MAXFALLSPEED, currentYvel);
			m_player->SetVelocity(0, currentYvel);
			m_player->SetPosition(currentXpos, currentYpos + currentYvel * GET_DELTATIME());

			if (m_deadTimer > PLAY_DEADINCOUNT_01_TIMER)
			{
				m_deadTimer = 0.0f;
				m_life--;
				Initialize();
			}
		}
		break;
	}
	case Goal:
	{
		if (m_goalIncount == 0)
		{
			m_player->SetVelocity(0, 0);
			m_player->ForceChangePlayerShape(PLAYER_SPR_S_GRAB);
			m_goalTimer += GET_DELTATIME();

			if (m_goalTimer > PLAY_GOALINCOUNT_00_TIMER)
			{
				m_goalTimer = 0.0f;
				m_goalIncount++;
			}
		}
		else if (m_goalIncount == 1)
		{
			m_player->SetPosition(m_player->GetPosX(), m_player->GetPosY() + (PLAYER_MAXFALLSPEED >> 1) * GET_DELTATIME());
			m_player->CheckGround();
			m_player->OnlyCheckStaticCollision();

			for (auto tile : m_tiles)
			{
				if (!tile.first->GetActive())
					continue;

				tile.first->Update();
			}

			if (m_player->IsGround())
				m_goalIncount++;
		}
		else if (m_goalIncount == 2)
		{
			m_goalTimer += GET_DELTATIME();
			if (m_goalTimer > PLAY_GOALINCOUNT_02_TIMER)
			{
				m_goalTimer = 0.0f;
				m_goalIncount++;
				m_player->ForceChangePlayerShape(PLAYER_SPR_S_WALK1);
			}
		}
		else if (m_goalIncount == 3)
		{
			m_player->SetForceInput(true, 1, true);
			m_player->UpdateMovement();
			m_player->MoveViewport();

			if (m_timer > 0.0f)
			{
				m_timer -= 85.0f * GET_DELTATIME();
				m_score += 85.0f * 20 * GET_DELTATIME();
			}

			if (m_timer < 0.0f && m_player->GetPosX() > GET_ANCHOR_POS().X + GET_SCREEN_WIDTH())
			{
				m_timer = 0.0f;
				m_goalIncount++;
			}
		}
		else if (m_goalIncount == 4)
		{
			m_goalTimer += GET_DELTATIME();
			if (m_goalTimer > PLAY_GOALINCOUNT_02_TIMER)
			{
				//골 종료
				m_goalIncount++;
				SET_ANCHOR_POS(0, 0);
				dynamic_cast<SetRankScene*>(GET_SCENE(SetRank))->SetCurrentScore((int)m_score);
				CHANGE_SCENE(SetRank);
			}
		}

		break;
	}
	}

}


void MyGame::PlayScene::Render()
{

	switch (gameState)
	{
	case PrintLife:
	{
		swprintf_s(m_msgBuffer, 1024, L"X %d  ", m_life);
		RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 + 3), (SHORT)(GET_SCREEN_HEIGHT() / 2 - 10) }, m_msgBuffer);
		RENDER_SPR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 5), (SHORT)(GET_SCREEN_HEIGHT() / 2 - 11) }, m_player->GetSprite());
		break;
	}
	case Playing:
	case PlayerDead:
	case Goal:
	{
		for (int i = 0; i < TILE_SPR_SIZE_Y * MAP01_SIZE_Y; i++)
		{
			COORD pos = { 0, (short)i };
			const wchar_t* d = m_renderedMap[i];

			Engine::GetInstance()->GetConsoleRenderer()->WStringDraw(pos, d, MAP01_SIZE_X * TILE_SPR_SIZE_X);
		}

		//m_acitve가 true인 오브젝트만 렌더링
		for (auto tile : m_tiles)
		{
			if (tile.first->GetActive())
			{
				RENDER_SPR({ (SHORT)(ceil(tile.first->GetPosX())), (SHORT)(ceil(tile.first->GetPosY())) }, tile.first->GetSprite());
			}
		}



		RENDER_SPR({ (SHORT)(ceil(m_player->GetPosX())), (SHORT)(ceil(m_player->GetPosY())) }, m_player->GetSprite());

		DebugPos = { GET_ANCHOR_POS().X, 0 };
		swprintf_s(DebugMsg, 1024, L"{ %f } ", m_player->GetVelX() + m_player->GetAdditionalVelX());
		RENDER_STR(DebugPos, DebugMsg);


		RENDER_STR({ GET_ANCHOR_POS().X, -9 }, L"    점수                                                                                                       시간    ");
		swprintf_s(m_msgBuffer, 1024, L"  %08d                                                                                                     %04d    ", (int)m_score, (int)m_timer);
		RENDER_STR({ GET_ANCHOR_POS().X, -8 }, m_msgBuffer);
		break;
	}
	}
}



MyGame::PlayScene::~PlayScene()
{
	for (int i = 0; i < TILE_SPR_SIZE_Y * MAP01_SIZE_Y; i++) {
		delete[] m_renderedMap[i];  // 내부 배열 해제
	}
	delete[] m_renderedMap;  // 외부 배열 해제
	
	for (auto tile = m_tiles.begin(); tile != m_tiles.end(); ) {
		if (tile->second) {
			//인스턴스인 경우 삭제
			delete tile->first;  // 메모리 해제
			tile = m_tiles.erase(tile);  // 원소 삭제 후 반복문 인덱스 조정
		}
		else {
			++tile;
		}
	}
	delete[] m_msgBuffer;
	delete m_player;
}


