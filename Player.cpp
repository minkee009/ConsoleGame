#include "Player.hpp"
#include "engine.hpp"

using MyGame::Engine;

MyGame::Player::Player(PlayScene* scene)
{
	m_isGrounded = false;
	m_jumpTimer = 0.0f;
	m_jumpTrigger = false;
	m_posX = 0;
	m_posY = 0;
	m_velX = 0.0f;
	m_velY = 0.0f;

	m_step = 0.0f;

	m_currentSprNum = 0;

	m_scene = scene;
	m_active = true;

	m_spr =
	{
		{ PLAYER_SPR_SIZE_X, PLAYER_SPR_SIZE_Y },
		{ PLAYER_SPR_PIVOT_X, PLAYER_SPR_PIVOT_Y },
		m_playerShape1,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};

}

MyGame::Player::~Player()
{
}

void MyGame::Player::Initialize()
{
	m_isGrounded = true;
	m_jumpTimer = 0.0f;
	m_jumpTrigger = false;
	m_posX = 0;
	m_posY = 0;
	m_velX = 0.0f;
	m_velY = 0.0f;

	m_step = 0.0f;

	m_currentSprNum = 0;
}

void MyGame::Player::UpdateMovement()
{
	if (!m_active) return;

	int playerMapXidx = (int)(m_posX < 0.0f ? m_posX - MAP01_SPR_SIZE_X : m_posX) / MAP01_SPR_SIZE_X;
	int playerMapYidx = (int)(m_posY < 0.0f ? m_posY - MAP01_SPR_SIZE_Y : m_posY) / MAP01_SPR_SIZE_Y;

	//바닥 체크
	//발 밑 3칸중 땅이 있는지 체크 후 발과 땅의 거리 계산
	auto lastGrounded = m_isGrounded;
	m_isGrounded = false;
	float playerFootY = m_posY + PLAYER_SPR_SIZE_Y;

	if (m_velY >= -0.001f)
		for (int i = playerMapYidx + 1; i <= playerMapYidx + 2; i++)
			for (int j = playerMapXidx; j <= playerMapXidx + 2; j++)
			{
				if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
					continue;

				auto pickY = (i)*MAP01_SPR_SIZE_Y - 0.02f;//타일의 y좌표
				auto pick = m_scene->GetTilemap()[i][j];	 //현재 타일
				auto pickXMin = j * MAP01_SPR_SIZE_X;        //타일의 x좌표 최소값
				auto pickXMax = (j + 1) * MAP01_SPR_SIZE_X;  //타일의 x좌표 최대값

				if ((m_scene->CheckTileCollision(pick))
					&& ((pickXMax > m_posX && pickXMin < m_posX + PLAYER_SPR_SIZE_X)
						|| (pickXMin < m_posX + PLAYER_SPR_SIZE_X && pickXMax >= m_posX))
					&& pickY < m_posY + PLAYER_SPR_SIZE_Y)
				{
					m_isGrounded = true;
					m_velY = 0.0f;
					break;
				}
			}

	if (!lastGrounded && m_isGrounded)
		m_spr.ShapeString = m_playerShape1;

	//속도 계산
	auto hInput = (GET_KEY(VK_RIGHT) ? 1 : 0) - (GET_KEY(VK_LEFT) ? 1 : 0);
	auto vInput = (GET_KEY(VK_DOWN) ? 1 : 0) - (GET_KEY(VK_UP) ? 1 : 0);

	if (m_isGrounded)
	{
		if (m_step > PLAYER_STEPRATE)
		{
			m_currentSprNum++;
			m_currentSprNum %= PLAYER_SPR_ANIM_MAXFRAME;

			switch (m_currentSprNum)
			{
			case 0:
				m_spr.ShapeString = m_playerShape1;
				break;
			case 1:
				m_spr.ShapeString = m_playerShape2;
				break;
			}

			m_step = 0.0f;
		}
		if (hInput != 0)
		{
			m_step += GET_DELTATIME();
			m_spr.Flip = hInput > 0 ? false : true;
			m_velX += hInput * PLAYER_ACCEL * ((hInput * m_velX < 0) ? 2.0f : 1.0f) * (GET_KEY(VK_LSHIFT) ? 2.0f : 1.0f) * GET_DELTATIME();
			m_velX = max(-PLAYER_MAXSPEED * (GET_KEY(VK_LSHIFT) ? 2.0f : 1.0f), min(PLAYER_MAXSPEED * (GET_KEY(VK_LSHIFT) ? 2.0f : 1.0f), m_velX));
		}
		else
		{
			m_velX = m_velX > 0 ? max(0.0f, m_velX - PLAYER_DECEL * GET_DELTATIME()) : min(0.0f, m_velX + PLAYER_DECEL * GET_DELTATIME());
		}

		if (GET_KEY_DOWN(VK_SPACE))
		{
			m_jumpTrigger = true;
			m_velY = -PLAYER_JUMPVEL;
			m_spr.ShapeString = m_playerShape3;
		}
	}
	else
	{
		m_velX += hInput * PLAYER_AIR_ACCEL * GET_DELTATIME();
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
			m_velY += PLAYER_GRAVITY * GET_DELTATIME();
			m_velY = min(PLAYER_MAXFALLSPEED, m_velY);
		}
	}


	m_posX += m_velX * GET_DELTATIME();
	m_posY += m_velY * GET_DELTATIME();


	//플레이어가 화면 중앙을 넘어갈 경우 뷰포트 이동
	if ((SHORT)m_posX + (m_spr.Size.X >> 1) - GET_ANCHOR_POS().X > (GET_SCREEN_WIDTH() >> 1))
	{
		SET_ANCHOR_POS((SHORT)m_posX + (m_spr.Size.X >> 1) - (GET_SCREEN_WIDTH() >> 1), GET_ANCHOR_POS().Y);
	}

	//맵의 끝이 화면 오른쪽에 있는 경우 뷰포트 제한
	if (MAP01_SIZE_X * MAP01_SPR_SIZE_X - GET_ANCHOR_POS().X < GET_SCREEN_WIDTH())
	{
		SET_ANCHOR_POS(MAP01_SIZE_X * MAP01_SPR_SIZE_X - GET_SCREEN_WIDTH(), GET_ANCHOR_POS().Y);
	}

	//뷰포트 왼쪽 벽 처리
	if ((SHORT)m_posX < GET_ANCHOR_POS().X)
	{
		m_posX = GET_ANCHOR_POS().X;
	}

	//뷰포트 오른쪽 벽 처리
	if ((SHORT)m_posX + PLAYER_SPR_SIZE_X > GET_ANCHOR_POS().X + GET_SCREEN_WIDTH() - 1)
	{
		m_posX = GET_ANCHOR_POS().X + GET_SCREEN_WIDTH() - PLAYER_SPR_SIZE_X - 1;
	}
}

void MyGame::Player::UpdateCollision()
{
	if (!m_active) return;
	//맵 충돌 체크
	int playerMapXidx = (int)(m_posX < 0.0f ? m_posX - MAP01_SPR_SIZE_X : m_posX) / MAP01_SPR_SIZE_X;
	int playerMapYidx = (int)(m_posY < 0.0f ? m_posY - MAP01_SPR_SIZE_Y : m_posY) / MAP01_SPR_SIZE_Y;

	float debugA = m_posY;
	float debugB = 0.0f;

	for (int i = playerMapYidx; i <= playerMapYidx + 2; i++)
		for (int j = playerMapXidx; j <= playerMapXidx + 2; j++)
		{
			if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
				continue;

			if (m_scene->CheckTileCollision(m_scene->GetTilemap()[i][j]) &&
				!((m_posX + PLAYER_SPR_SIZE_X < j * MAP01_SPR_SIZE_X)
					|| (m_posX > (j + 1) * MAP01_SPR_SIZE_X)
					|| (m_posY + PLAYER_SPR_SIZE_Y < i * MAP01_SPR_SIZE_Y)
					|| (m_posY > (i + 1) * MAP01_SPR_SIZE_Y)))
			{

				//침투 계산
				float penetrationX = min(m_posX + PLAYER_SPR_SIZE_X - j * MAP01_SPR_SIZE_X, (j + 1) * MAP01_SPR_SIZE_X - m_posX);
				float penetrationY = min(m_posY + PLAYER_SPR_SIZE_Y - i * MAP01_SPR_SIZE_Y, (i + 1) * MAP01_SPR_SIZE_Y - m_posY);

				// X축과 Y축 중 더 작은 침투 방향으로 이동하여 충돌 해결
				if (penetrationX < penetrationY) {
					if (m_posX < j * MAP01_SPR_SIZE_X)
					{
						auto prevPosX = m_posX;
						m_posX -= penetrationX; // 왼쪽으로 밀기
						if (abs(m_posX - prevPosX) >= 0.000001)
							m_posX = round(m_posX);
					}
					else
						m_posX += penetrationX; // 오른쪽으로 밀기

					if (m_posY + PLAYER_SPR_SIZE_Y - 0.5 >= i * MAP01_SPR_SIZE_Y
						&& m_posY + 0.5 < (i + 1) * MAP01_SPR_SIZE_Y)
						m_velX = 0.0f;

				}
				else {
					if (m_posY < i * MAP01_SPR_SIZE_Y)
					{
						m_posY = i * MAP01_SPR_SIZE_Y - PLAYER_SPR_SIZE_Y;// 위로 밀기
					}
					else
					{
						m_posY += penetrationY; // 아래로 밀기
						m_jumpTrigger = false;
						m_jumpTimer = 0.0f;
						m_velY = 0.0f;
					}
				}
			}
		}
}

