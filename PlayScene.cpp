#include "PlayScene.hpp"
#include "engine.hpp"
#include "SetRankScene.hpp"
#include <cwchar>
#include <algorithm> 
#include "math.hpp"
#include "ObjectManager.hpp"

////--오브젝트
//타일
#include "GoalPole.hpp"
#include "GoalFlag.hpp"
#include "Block.hpp"
#include "MysteryBlock.hpp"

//아이템
#include "JumpCoin.hpp"
#include "Coin.hpp"
#include "DeathShroom.hpp"

//적
#include "GoomBa.hpp"
#include "Jumper.hpp"
#include "Ninja.hpp"
#include "DummyGoomba.hpp"



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
	m_objManager = new ObjectManager;
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
				goalPole->SetSpawnPos(spawnPosX, spawnPosY);
				goalPole->SetPosition(spawnPosX, spawnPosY);
				goalPole->Initialize();


				m_objManager->tiles.push_back({ goalPole, false });

				auto goalFlag = new GoalFlag(this);
				spawnPosX += 2;
				goalFlag->SetSpawnPos(spawnPosX, spawnPosY);
				goalFlag->SetPosition(spawnPosX, spawnPosY);
				goalFlag->Initialize();

				m_objManager->tiles.push_back({ goalFlag, false });
				break;
			}
			case 'e':
			{
				auto goomba = new GoomBa(this);
				auto spawnPosX = j * TILE_SPR_SIZE_X;
				auto spawnPosY = i * TILE_SPR_SIZE_Y + CorrectPosY(goomba->GetSprite()->Size.Y);
				
				goomba->SetSpawnPos(spawnPosX, spawnPosY);
				goomba->SetPosition(spawnPosX, spawnPosY);
				goomba->Initialize();

				m_objManager->enemys.push_back({ goomba, false });
				break;
			}
			case 'F':
			{
				auto jumper = new Jumper(this);
				auto spawnPosX = j * TILE_SPR_SIZE_X + 2;
				auto spawnPosY = i * TILE_SPR_SIZE_Y + CorrectPosY(jumper->GetSprite()->Size.Y);

				jumper->SetSpawnPos(spawnPosX, spawnPosY);
				jumper->SetPosition(spawnPosX, spawnPosY);
				jumper->Initialize();

				m_objManager->enemys.push_back({ jumper, false });
				break;
			}
			case 'N':
			{
				auto ninja = new Ninja(this);
				auto spawnPosX = j * TILE_SPR_SIZE_X + 2;
				auto spawnPosY = i * TILE_SPR_SIZE_Y + CorrectPosY(ninja->GetSprite()->Size.Y);

				ninja->SetSpawnPos(spawnPosX, spawnPosY);
				ninja->SetPosition(spawnPosX, spawnPosY);
				ninja->Initialize();

				m_objManager->enemys.push_back({ ninja, false });
				break;
			}
			case 'D':
			{
				auto dgoomba = new DummyGoomBa(this);
				auto spawnPosX = j * TILE_SPR_SIZE_X;
				auto spawnPosY = i * TILE_SPR_SIZE_Y + CorrectPosY(dgoomba->GetSprite()->Size.Y);

				dgoomba->SetSpawnPos(spawnPosX, spawnPosY);
				dgoomba->SetPosition(spawnPosX, spawnPosY);
				dgoomba->Initialize();

				m_objManager->enemys.push_back({ dgoomba, false });
				break;
			}
			case 'B':
			{
				auto block = new Block(this);
				auto spawnPosX = j * TILE_SPR_SIZE_X;
				auto spawnPosY = i * TILE_SPR_SIZE_Y + CorrectPosY(block->GetSprite()->Size.Y);

				block->SetSpawnPos(spawnPosX, spawnPosY);
				block->SetPosition(spawnPosX, spawnPosY);
				block->Initialize();

				m_objManager->tiles.push_back({ block, false });
				break;
			}
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '~':
			case '!':
			case '@':
			case '#':
			case '$':
			case '%':
			{
				auto block = new MysteryBlock(this);

				char currentChar = m_map01[i][j];

				//coin
				if(currentChar == '0' || currentChar == '~')
					block->SetInItem(new JumpCoin(this));
				if (currentChar == '1' || currentChar == '!')
					block->SetInEnemy(new GoomBa(this));
				if (currentChar == '2' || currentChar == '@')
					block->SetInEnemy(new DummyGoomBa(this));
				if (currentChar == '3' || currentChar == '#')
					block->SetInEnemy(new Jumper(this));
				if (currentChar == '4' || currentChar == '$')
					block->SetInEnemy(new Ninja(this));
				if (currentChar == '5' || currentChar == '%')
					block->SetInItem(new DeathShroom(this));

				auto spawnPosX = j * TILE_SPR_SIZE_X;
				auto spawnPosY = i * TILE_SPR_SIZE_Y + CorrectPosY(block->GetSprite()->Size.Y);

				if(currentChar == '~' || currentChar == '!' 
					|| currentChar == '@' || currentChar == '#' 
					|| currentChar == '$' || currentChar == '%')
					block->SetHide();
				
				block->SetSpawnPos(spawnPosX, spawnPosY);
				block->SetPosition(spawnPosX, spawnPosY);
				block->Initialize();

				m_objManager->tiles.push_back({ block, false });
				break;
			}
			case 'C':
			{
				auto coin = new Coin(this);
				auto spawnPosX = j * TILE_SPR_SIZE_X;
				auto spawnPosY = i * TILE_SPR_SIZE_Y + CorrectPosY(coin->GetSprite()->Size.Y);

				coin->SetSpawnPos(spawnPosX, spawnPosY);
				coin->SetPosition(spawnPosX, spawnPosY);
				coin->Initialize();

				m_objManager->items.push_back({ coin, false });
				break;
			}
			case 'H':
			{
				auto shroom = new DeathShroom(this);
				auto spawnPosX = j * TILE_SPR_SIZE_X;
				auto spawnPosY = i * TILE_SPR_SIZE_Y + CorrectPosY(shroom->GetSprite()->Size.Y);

				shroom->SetSpawnPos(spawnPosX, spawnPosY);
				shroom->SetPosition(spawnPosX, spawnPosY);
				shroom->Initialize();

				m_objManager->items.push_back({ shroom, false });
				break;
			}
			}
		}
	}

	m_player = new MyGame::Player(this);
	m_pointPrinter = new PointPrinter;
}

void MyGame::PlayScene::Initialize()
{
	Engine::GetInstance()->GetConsoleRenderer()->SetViewPortAnchor(0, -10);
	
	//메모리 관리 (해제) 및 초기화
	//타일
	for (auto tile = m_objManager->tiles.begin(); tile != m_objManager->tiles.end(); ) {
		if (tile->second) {
			//인스턴스인 경우 삭제
			delete tile->first;  // 메모리 해제
			tile = m_objManager->tiles.erase(tile);  // 원소 삭제 후 반복문 인덱스 조정
		}
		else {
			tile->first->Initialize();
			++tile;
		}
	}

	//아이템
	for (auto item = m_objManager->items.begin(); item != m_objManager->items.end(); ) {
		if (item->second) {
			//인스턴스인 경우 삭제
			delete item->first;  // 메모리 해제
			item = m_objManager->items.erase(item);  // 원소 삭제 후 반복문 인덱스 조정
		}
		else {
			item->first->Initialize();
			++item;
		}
	}

	//에너미
	for (auto enemy = m_objManager->enemys.begin(); enemy != m_objManager->enemys.end(); ) {
		if (enemy->second) {
			//인스턴스인 경우 삭제
			delete enemy->first;  // 메모리 해제
			enemy = m_objManager->enemys.erase(enemy);  // 원소 삭제 후 반복문 인덱스 조정
		}
		else {
			enemy->first->Initialize();
			++enemy;
		}
	}

	//게임 변수 초기화
	m_goalTimer = 0;
	m_goalIncount = 0;
	m_deadIncount = 0;
	m_deadTimer = 0;
	m_score = 0.0f;
	m_player->Initialize();
	m_player->SetPosition(2, 26);

	m_pause = false;
	m_timer = 0;
	m_menuIdx = 0;
	gameState = PrintLife;
	m_pointPrinter->ClearAllPoints();
}

void MyGame::PlayScene::Update()
{
	if (!m_childMode && m_life == 0)
	{
		//게임 오버
		CHANGE_SCENE(GameOver);
		SET_ANCHOR_POS(0, 0);

		m_life = 3;
	}

	if (GET_KEY_DOWN(VK_ESCAPE))
	{
		m_pause = true;
	}

////--중간 추가생성된 오브젝트 관리
	//타일
	while (!m_objManager->addedTiles.empty())
	{
		m_objManager->tiles.push_back({ m_objManager->addedTiles.front(),true });
		m_objManager->addedTiles.pop();
	}

	//아이템
	while (!m_objManager->addedItems.empty())
	{
		m_objManager->items.push_back({ m_objManager->addedItems.front(),true });
		m_objManager->addedItems.pop();
	}

	//적
	while (!m_objManager->addedEnemys.empty())
	{
		m_objManager->enemys.push_back({m_objManager->addedEnemys.front(),true});
		m_objManager->addedEnemys.pop();
	}

	if (m_pause)
	{
		SET_ANCHOR_POS(0, 0);
		if (GET_KEY_DOWN(VK_UP))
			m_menuIdx--;

		if (GET_KEY_DOWN(VK_DOWN))
			m_menuIdx++;

		m_menuIdx = m_menuIdx < 0 ? 2 - 1 : m_menuIdx;
		m_menuIdx %= 2;

		if (GET_KEY_DOWN(VK_RETURN) || GET_KEY_DOWN(VK_SPACE))
		{
			switch (m_menuIdx)
			{
			case 0:
				SET_ANCHOR_POS(0,-10);
				m_player->MoveViewport();
				m_pause = false;
				break;
			case 1:
				m_life = 3;
				CHANGE_SCENE(Menu);
				break;
			}
		}

		return;
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

		if (!m_childMode && m_timer < 0.0f)
		{
			gameState = PlayerDead;
			return;
		}

		if (m_player->GetPosY() > GET_ANCHOR_POS().Y + GET_SCREEN_HEIGHT() + PLAYER_SPR_SIZE_Y + 3)
		{
			//추락 죽음
			gameState = PlayerDead;
		}

		//플레이어 움직임을 먼저 업데이트
		m_player->UpdateMovement();
		m_player->MoveViewport();
		m_player->ClampPosToViewport();

	////--오브젝트 m_active관리
		//타일 관리
		for (auto& tile : m_objManager->tiles)
		{
			if (tile.first->IsDeleted())
			{
				tile.first->SetActive(false);
				continue;
			}

			//타일이 화면안에 들어오는지 체크
			if (!CheckAABB(tile.first->GetBbox(), GET_SCREEN_BBOX()))
			{
				tile.first->SetActive(false);
			}
			else
			{
				tile.first->SetActive(true);
			}
		}

		//아이템 관리
		for (auto& item : m_objManager->items)
		{
			if (!item.first->IsAlive())
			{
				item.first->SetActive(false);
				continue;
			}

			//타일이 화면안에 들어오는지 체크
			if (!CheckAABB(item.first->GetBbox(), GET_SCREEN_BBOX()))
			{
				item.first->SetActive(false);
			}
			else
			{
				item.first->SetActive(true);
			}
		}

		//적 관리
		for (auto& enemy : m_objManager->enemys)
		{
			if (!enemy.first->IsAlive())
			{
				enemy.first->SetActive(false);
				continue;
			}

			//적이 화면안에 들어오는지 체크
			if (!CheckAABB(enemy.first->GetBbox(), GET_SCREEN_BBOX()))
			{
				enemy.first->SetActive(false);
			}
			else
			{
				enemy.first->SetActive(true);
			}
		}



	////--오브젝트 업데이트

		//타일 업데이트
		for (auto& tile : m_objManager->tiles)
		{
			if (tile.first->GetActive())
				tile.first->Update();
		}

		//아이템 업데이트
		for (auto& item : m_objManager->items)
		{
			if (item.first->GetActive() && !item.first->GetForceIgnoreCollision())
				item.first->Update();
		}

		//적 업데이트
		for (auto& enemy : m_objManager->enemys)
		{
			if (enemy.first->GetActive() && !enemy.first->GetForceIgnoreCollision())
				enemy.first->Update();
		}

	////--충돌체크

		//아이템
		for (auto& item : m_objManager->items)
		{
			if (item.first->GetActive() && !item.first->GetForceIgnoreCollision())
				item.first->CheckCollision();
		}

		//적
		for (auto& enemy : m_objManager->enemys)
		{
			if (enemy.first->GetActive() && !enemy.first->GetForceIgnoreCollision() && !enemy.first->IsActtacked())
				enemy.first->CheckCollision();
		}

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
			m_player->SetSpriteFlip(false);
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

			for (auto tile : m_objManager->tiles)
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

				if (m_childMode)
				{
					CHANGE_SCENE(GameOver);
				}
				else
				{
					dynamic_cast<SetRankScene*>(GET_SCENE(SetRank))->SetCurrentScore((int)m_score);
					CHANGE_SCENE(SetRank);
				}

				m_life = 3;
			}
		}

		break;
	}
	}

}


void MyGame::PlayScene::Render()
{
	if (m_pause)
	{
		short menuPointerY = 0;
		switch (m_menuIdx)
		{
		case 0:
		{
			menuPointerY = (SHORT)(GET_SCREEN_HEIGHT() / 2 - 2);
			break;
		}
		case 1:
		{
			menuPointerY = (SHORT)(GET_SCREEN_HEIGHT() / 2);
			break;
		}
		}
		RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 5), menuPointerY }, L">");
		RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 6), (SHORT)(GET_SCREEN_HEIGHT() / 2 - 5) }, L"** 일시정지 **");
		RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 3), (SHORT)(GET_SCREEN_HEIGHT() / 2 - 2) }, L"재개");
		RENDER_STR({ (SHORT)(GET_SCREEN_WIDTH() / 2 - 3), (SHORT)(GET_SCREEN_HEIGHT() / 2) }, L"메인메뉴");
		return;
	}


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
		m_pointPrinter->UpdatePoints();

		for (int i = 0; i < TILE_SPR_SIZE_Y * MAP01_SIZE_Y; i++)
		{
			COORD pos = { 0, (short)i };
			const wchar_t* d = m_renderedMap[i];

			Engine::GetInstance()->GetConsoleRenderer()->WStringDraw(pos, d, MAP01_SIZE_X * TILE_SPR_SIZE_X);
		}

		//m_acitve가 true인 오브젝트(+ 살아있는)만 렌더링
		for (auto& tile : m_objManager->tiles)
		{
			if (tile.first->GetActive())
			{
				RENDER_SPR({ (SHORT)(ceil(tile.first->GetPosX())), (SHORT)(ceil(tile.first->GetPosY())) }, tile.first->GetSprite());
			}
		}

		RENDER_STR({ 4,20 }, L"shift - 달리기, space - 점프, 방향키 - 이동");

		for (auto& item : m_objManager->items)
		{
			if (item.first->GetActive())
			{
				RENDER_SPR({ (SHORT)(ceil(item.first->GetPosX())), (SHORT)(ceil(item.first->GetPosY())) }, item.first->GetSprite());
			}
		}

		for (auto& enemy : m_objManager->enemys)
		{
			if (enemy.first->GetActive())
			{
				RENDER_SPR({ (SHORT)(ceil(enemy.first->GetPosX())), (SHORT)(ceil(enemy.first->GetPosY())) }, enemy.first->GetSprite());
			}
		}

		for (auto& point : *m_pointPrinter->GetPoints())
		{
			RENDER_STR({ (SHORT)ceil(point.posX), (SHORT)ceil(point.posY) }, point.point);
		}

		RENDER_SPR({ (SHORT)(ceil(m_player->GetPosX())), (SHORT)(ceil(m_player->GetPosY())) }, m_player->GetSprite());

		//DebugPos = { GET_ANCHOR_POS().X, 0 };
		//swprintf_s(DebugMsg, 1024, L"{ %f } ", 1 / GET_DELTATIME());
		//RENDER_STR(DebugPos, DebugMsg);

		RENDER_STR({ GET_ANCHOR_POS().X, -9 }, L"    점수                                                                                                       시간    ");
		
		if (!m_childMode)
		{
			swprintf_s(m_msgBuffer, 1024, L"  %08d                                                                                                     %04d    ", (int)m_score, (int)m_timer);
		}
		else
		{
			swprintf_s(m_msgBuffer, 1024, L"  %08d   ", (int)m_score);
		}
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
	
	for (auto& tile : m_objManager->tiles) {
		if (tile.first) {
			delete tile.first;  // 동적 할당된 Tile*을 해제
			tile.first = nullptr;  
		}
	}

	for (auto& item : m_objManager->items) {
		if (item.first) {
			delete item.first;
			item.first = nullptr;
		}
	}

	for (auto& enemy : m_objManager->enemys) {
		if (enemy.first) {
			delete enemy.first;  
			enemy.first = nullptr;
		}
	}

	while (!m_objManager->addedTiles.empty())
	{
		delete m_objManager->addedTiles.front();
		m_objManager->addedTiles.pop();
	}
	while (!m_objManager->addedEnemys.empty())
	{
		delete m_objManager->addedEnemys.front();
		m_objManager->addedEnemys.pop();
	}

	delete m_objManager;
	delete[] m_msgBuffer;
	delete m_player;
	delete m_pointPrinter;
}


