#include "PlayScene.hpp"
#include "engine.hpp"
#include <cwchar>

using MyGame::Engine;

wchar_t DebugMsg[1024];

COORD DebugPos = { 0, 0 };

PlayScene::PlayScene()
{
	m_renderedMap = new WCHAR*[MAP01_SPR_SIZE_Y * MAP01_SIZE_Y];
	for (int i = 0; i < MAP01_SPR_SIZE_Y * MAP01_SIZE_Y; i++)
		m_renderedMap[i] = new WCHAR[MAP01_SPR_SIZE_X * MAP01_SIZE_X + 1];
}

void PlayScene::Initialize()
{
	Engine::GetInstance()->GetConsoleRenderer()->SetViewPortCenter(0, -16);

	m_isGrounded = true;

	m_playerPosX = 0;
	m_playerPosY = 18;

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

	// 사전 렌더링
	for (int i = 0; i < MAP01_SIZE_Y; i++) {
		for (int j = 0; j < MAP01_SIZE_X; j++) {
			switch (m_map01[i][j]) {
			case 'g':
				// 'g'일 경우
				for (int k = 0; k < MAP01_SPR_SIZE_Y; k++) {
					// 각 행에 'g' 관련 데이터를 복사
					wmemcpy(&m_renderedMap[i * MAP01_SPR_SIZE_Y + k][j * MAP01_SPR_SIZE_X], m_map01_groundSpr.ShapeString[k], MAP01_SPR_SIZE_X);
				}
				break;
			case 'd':
				// 'd'일 경우
				for (int k = 0; k < MAP01_SPR_SIZE_Y; k++) {
					// 각 행에 'd' 관련 데이터를 복사
					wmemcpy(&m_renderedMap[i * MAP01_SPR_SIZE_Y + k][j * MAP01_SPR_SIZE_X], m_map01_dgroundSpr.ShapeString[k], MAP01_SPR_SIZE_X);
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
	auto hInput = (GET_KEY(VK_RIGHT) ? 1 : 0) - (GET_KEY(VK_LEFT) ? 1 : 0);
	auto vInput = (GET_KEY(VK_DOWN) ? 1 : 0) - (GET_KEY(VK_UP) ? 1 : 0);

	if (hInput != 0)
	{
		m_playerSpr.Flip = hInput > 0 ? false : true;
		m_player_velX += hInput * PLAYER_ACCEL * ((hInput * m_player_velX < 0) ? 2.0f : 1.0f) * (GET_KEY(VK_LSHIFT) ? 2.0f : 1.0f) * GET_DELTATIME();
		m_player_velX = max(-PLAYER_MAXSPEED * (GET_KEY(VK_LSHIFT) ? 2.0f : 1.0f), min(PLAYER_MAXSPEED * (GET_KEY(VK_LSHIFT) ? 2.0f : 1.0f), m_player_velX));
	}
	else
	{
		m_player_velX = m_player_velX > 0 ? max(0.0f, m_player_velX - PLAYER_DECEL * GET_DELTATIME()) : min(0.0f, m_player_velX + PLAYER_DECEL * GET_DELTATIME());
	}
		
	m_playerPosX += m_player_velX * GET_DELTATIME();
	m_playerPosY += vInput * 12.0f * GET_DELTATIME();

	if (hInput != 0)
		m_playerStep += GET_DELTATIME();

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

	//맵 충돌 체크
	int playerMapXidx = (int)(m_playerPosX < 0.0f ? m_playerPosX - MAP01_SPR_SIZE_X : m_playerPosX) / MAP01_SPR_SIZE_X;
	int playerMapYidx = (int)(m_playerPosY < 0.0f ? m_playerPosY - MAP01_SPR_SIZE_Y : m_playerPosY) / MAP01_SPR_SIZE_Y;

	float debugA = m_playerPosY;
	float debugB = 0.0f;

	for (int i = playerMapYidx; i <= playerMapYidx + 2; i++)
		for (int j = playerMapXidx; j <= playerMapXidx + 2; j++)
		{
			if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
				continue;

			if ((m_map01[i][j] == 'g' || m_map01[i][j] == 'd') &&
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
						m_playerPosY = i * MAP01_SPR_SIZE_Y - PLAYER_SPR_SIZE_Y;// 아래로 밀기
					}
					else
						m_playerPosY += penetrationY; // 위로 밀기
				}					
				
				debugA = m_playerPosY + 0.5;
				debugB = (i + 1) * MAP01_SPR_SIZE_Y;
			}
		}

	DebugPos = { (SHORT)playerMapXidx * MAP01_SPR_SIZE_X,(short)playerMapYidx * MAP01_SPR_SIZE_Y };
	swprintf_s(DebugMsg, 1024, L"█ <- { %f , %f} ", debugA, debugB);

	//뷰포트 이동
	if ((SHORT)m_playerPosX + (m_playerSpr.Size.X >> 1) - GET_ANCHOR_POS().X > (GET_SCREEN_WIDTH() >> 1))
	{
		SET_ANCHOR_POS((SHORT)m_playerPosX + (m_playerSpr.Size.X >> 1) - (GET_SCREEN_WIDTH() >> 1), GET_ANCHOR_POS().Y);
	}

	//뷰포트 왼쪽 벽 처리
	if ((SHORT)m_playerPosX < GET_ANCHOR_POS().X)
	{
		m_playerPosX = GET_ANCHOR_POS().X;
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


	RENDER_STR(DebugPos, DebugMsg);
}
	
	

PlayScene::~PlayScene()
{
	if (m_renderedMap)
		delete[] m_renderedMap;
}


//void PlayScene::Render()
//{
//
//	const wchar_t* de = m_renderedMap[MAP01_SPR_SIZE_Y * MAP01_SIZE_Y - 1];
//		RENDER_STR({ 0,(short)MAP01_SPR_SIZE_Y * MAP01_SIZE_Y - 1 }, de);;
//	/*for (int i = 0; i < MAP01_SPR_SIZE_Y * MAP01_SIZE_Y; i++)
//		RENDER_STR({ 0,(short)i }, m_renderedMap[i]);*/
//
//	//swprintf_s(DebugMsg, 1024, L"█ <- %f fps", 1 / GET_DELTATIME());
//	RENDER_STR({ 0, -2 }, DebugMsg);
//
//	//RENDER_SPR({ (SHORT)(ceil(m_playerPosX)), (SHORT)(ceil(m_playerPosY)) }, &m_playerSpr);
//}



