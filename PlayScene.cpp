#include "PlayScene.hpp"
#include "engine.hpp"
#include <cwchar>

using MyGame::Engine;

wchar_t DebugMsg[1024];
wchar_t DebugMsg2[1024];
wchar_t DebugMsg3[1024];
wchar_t DebugMsg4[1024];
wchar_t DebugMsg5[1024];

float num_p = 0;

void PlayScene::Initialize()
{
	Engine::GetInstance()->GetConsoleRenderer()->SetViewPortCenter(0, -16);

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
		m_playerSpr.Flip = hInput > 0 ? false : true;

	m_playerPosX += hInput * 20.0f * GET_DELTATIME();
	m_playerPosY += vInput * 20.0f * GET_DELTATIME();

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



	int check = 0;

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
				check++;

				//보정
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
				}
				else {
					if (m_playerPosY < i* MAP01_SPR_SIZE_Y)
					{
						m_playerPosY = i * MAP01_SPR_SIZE_Y - PLAYER_SPR_SIZE_Y;// 아래로 밀기
					}
					else
						m_playerPosY += penetrationY; // 위로 밀기
				}
			}
		}

	m_playerSpr.Attribute = check > 0 ? FOREGROUND_RED : FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;




	num_p += 8.0f * GET_DELTATIME();

	if ((SHORT)(ceil(m_playerPosX)) > GET_ANCHOR_POS().X)
	{

	}

	//Engine::GetInstance()->GetConsoleRenderer()->SetViewPortCenter(num_p, -16);
}

void PlayScene::Render()
{

	for (int i = 0; i < MAP01_SPR_SIZE_Y * MAP01_SIZE_Y; i++)
		RENDER_STR({ 0,(short)i }, m_renderedMap[i]);

	swprintf_s(DebugMsg, 1024, L"█ <- %f fps", 1 / GET_DELTATIME());
	RENDER_STR({ 0, -2 }, DebugMsg);

	RENDER_SPR({ (SHORT)(ceil(m_playerPosX)), (SHORT)(ceil(m_playerPosY)) }, &m_playerSpr);
}



