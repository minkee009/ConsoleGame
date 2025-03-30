#include "Player.hpp"
#include "engine.hpp"
#include "math.hpp"

using MyGame::Engine;

MyGame::Player::Player(PlayScene* scene)
{
	Initialize();

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
	m_forceInput = false;
	m_forceInputDash = false;
	m_posX = 0;
	m_posY = 0;
	m_velX = 0.0f;
	m_velY = 0.0f;
	m_additianalVelX = 0.0f;
	m_step = 0.0f;

	m_currentSprNum = 0;
	ForceChangePlayerShape(PLAYER_SPR_S_WALK1);
}

void MyGame::Player::UpdateMovement()
{
	if (!m_active) return;

	int playerMapXidx = (int)(m_posX < 0.0f ? m_posX - TILE_SPR_SIZE_X : m_posX) / TILE_SPR_SIZE_X;
	int playerMapYidx = (int)(m_posY < 0.0f ? m_posY - TILE_SPR_SIZE_Y : m_posY) / TILE_SPR_SIZE_Y;

	//바닥 체크
	//발 밑 3칸중 땅이 있는지 체크 후 발과 땅의 거리 계산
	auto lastGrounded = m_isGrounded;
	CheckGround();

	//착지 시 그림 바꾸기
	if (!lastGrounded && m_isGrounded)
		m_spr.ShapeString = m_playerShape1;

	//공중가속
	if (lastGrounded && !m_isGrounded)
		m_additianalVelX = GET_KEY(VK_LSHIFT);

	//속도 계산
	auto hInput = ((GET_KEY(VK_RIGHT) ? 1 : 0) - (GET_KEY(VK_LEFT) ? 1 : 0));
	hInput = m_forceInput ? m_forceInputX : hInput;
	//auto vInput = ((GET_KEY(VK_DOWN) ? 1 : 0) - (GET_KEY(VK_UP) ? 1 : 0));

	if (m_isGrounded)
	{
		m_additianalVelX = false;
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
			m_step += (GET_KEY(VK_LSHIFT) || m_forceInputDash ? 2.0f : 1.0f) * GET_DELTATIME();
			m_spr.Flip = hInput > 0 ? false : true;
			m_velX += hInput * PLAYER_ACCEL * ((hInput * m_velX < 0) ? 3.0f : 1.0f) * (GET_KEY(VK_LSHIFT) || m_forceInputDash ? 2.0f : 1.0f) * GET_DELTATIME();
			m_velX = max(GET_KEY(VK_LSHIFT) || m_forceInputDash ? -PLAYER_DASH_MAXSPEED : -PLAYER_MAXSPEED, min((GET_KEY(VK_LSHIFT) || m_forceInputDash ? PLAYER_DASH_MAXSPEED : PLAYER_MAXSPEED), m_velX));
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
		m_velX = max(m_additianalVelX || m_forceInputDash ? -PLAYER_DASH_MAXSPEED : -PLAYER_MAXSPEED, min((m_additianalVelX || m_forceInputDash ? PLAYER_DASH_MAXSPEED : PLAYER_MAXSPEED), m_velX));
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
}

void MyGame::Player::MoveViewport()
{
	//플레이어가 화면 중앙을 넘어갈 경우 뷰포트 이동
	if ((SHORT)m_posX + (m_spr.Size.X >> 1) - GET_ANCHOR_POS().X > (GET_SCREEN_WIDTH() >> 1))
	{
		SET_ANCHOR_POS((SHORT)m_posX + (m_spr.Size.X >> 1) - (GET_SCREEN_WIDTH() >> 1), GET_ANCHOR_POS().Y);
	}

	//맵의 끝이 화면 오른쪽에 있는 경우 뷰포트 제한
	if (MAP01_SIZE_X * TILE_SPR_SIZE_X - GET_ANCHOR_POS().X < GET_SCREEN_WIDTH())
	{
		SET_ANCHOR_POS(MAP01_SIZE_X * TILE_SPR_SIZE_X - GET_SCREEN_WIDTH(), GET_ANCHOR_POS().Y);
	}
}

void MyGame::Player::ClampPosToViewport()
{
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

void MyGame::Player::CheckGround()
{
	int playerMapXidx = (int)(m_posX < 0.0f ? m_posX - TILE_SPR_SIZE_X : m_posX) / TILE_SPR_SIZE_X;
	int playerMapYidx = (int)(m_posY < 0.0f ? m_posY - TILE_SPR_SIZE_Y : m_posY) / TILE_SPR_SIZE_Y;

	m_isGrounded = false;
	float playerFootY = m_posY + PLAYER_SPR_SIZE_Y;

	if (m_velY >= -0.001f)
		for (int i = playerMapYidx + 1; i <= playerMapYidx + 2; i++)
			for (int j = playerMapXidx; j <= playerMapXidx + 2; j++)
			{
				if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
					continue;

				auto pickY = (i)*TILE_SPR_SIZE_Y - 0.02f;//타일의 y좌표
				auto pick = m_scene->GetTilemap()[i][j];	 //현재 타일
				auto pickXMin = j * TILE_SPR_SIZE_X;        //타일의 x좌표 최소값
				auto pickXMax = (j + 1) * TILE_SPR_SIZE_X;  //타일의 x좌표 최대값

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
}

void MyGame::Player::CheckCollision()
{
	if (!m_active) return;
	//맵 충돌 체크
	int playerMapXidx = (int)(m_posX < 0.0f ? m_posX - TILE_SPR_SIZE_X : m_posX) / TILE_SPR_SIZE_X;
	int playerMapYidx = (int)(m_posY < 0.0f ? m_posY - TILE_SPR_SIZE_Y : m_posY) / TILE_SPR_SIZE_Y;

	for (int i = playerMapYidx; i <= playerMapYidx + 2; i++)
		for (int j = playerMapXidx; j <= playerMapXidx + 2; j++)
		{
			if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
				continue;

			float p_minX = m_posX;
			float p_maxX = m_posX + PLAYER_SPR_SIZE_X;
			float p_minY = m_posY;
			float p_maxY = m_posY + PLAYER_SPR_SIZE_Y;

			float t_minX = j * TILE_SPR_SIZE_X;
			float t_maxX = (j + 1) * TILE_SPR_SIZE_X;
			float t_minY = i * TILE_SPR_SIZE_Y;
			float t_maxY = (i + 1) * TILE_SPR_SIZE_Y;

			int collisionFlag = 0;

			if (m_scene->CheckTileCollision(m_scene->GetTilemap()[i][j])
				&& MyGame::CheckAABB(p_maxX, p_maxY, p_minX, p_minY,
					t_maxX, t_maxY, t_minX, t_minY))
			{
				collisionFlag = ApplyPenetration(&m_posX, &m_posY, p_maxX, p_maxY, p_minX, p_minY,
					t_maxX, t_maxY, t_minX, t_minY);
			}

			if ( (MATH_COL_FLAG_PUSHLEFT | MATH_COL_FLAG_PUSHRIGHT) & collisionFlag)
			{
				if (p_maxY - 0.5 >= t_minY
					&& p_minY + 0.5 < t_maxY)
					m_velX = 0.0f;
			}
			if (collisionFlag & MATH_COL_FLAG_PUSHDOWN)
			{
				m_jumpTrigger = false;
				m_jumpTimer = 0.0f;
			}
		}

	Bbox p_checkBox = { m_posX - PLAYER_SPR_SIZE_X + (PLAYER_SPR_SIZE_X << 1), m_posY - PLAYER_SPR_SIZE_Y + (PLAYER_SPR_SIZE_X << 1) , m_posX - PLAYER_SPR_SIZE_X , m_posY - PLAYER_SPR_SIZE_Y };

	//타일 충돌
	for (auto& tile : *(m_scene->GetTiles()))
	{
		if (!tile.first->GetActive())
			continue;

		//경계범위 체크
		if (CheckAABB(p_checkBox,
			tile.first->GetBbox()))
		{
			bool isCollide = CheckAABB(GetBbox(),
				tile.first->GetBbox());

			if (isCollide)
			{
				tile.first->CallInteract(ApplyPenetration(&m_posX, &m_posY, GetBbox(),
					tile.first->GetBbox()));
			}
		}
	}

	//아이템 충돌

	//적 충돌
	for (auto& enemy : *(m_scene->GetEnemys()))
	{
		if (!enemy.first->GetActive() 
			|| enemy.first->IsActtacked()
			|| !enemy.first->IsAlive())
			continue;

		//경계범위 체크
		if (CheckAABB(p_checkBox,
			enemy.first->GetBbox()))
		{
			bool isCollide = CheckAABB(GetBbox(),
				enemy.first->GetBbox());

			if (isCollide)
			{
				auto collisionFlag = CalcPenetration(GetBbox(),
					enemy.first->GetBbox());

				if (collisionFlag & (MATH_COL_FLAG_PUSHRIGHT | MATH_COL_FLAG_PUSHLEFT | MATH_COL_FLAG_PUSHDOWN) || enemy.first->IsDontPress())
				{
					m_scene->gameState = PlayerDead;
					break;
				}

				if (collisionFlag & MATH_COL_FLAG_PUSHUP)
				{
					m_velY = -(PLAYER_JUMPVEL);
					m_jumpTrigger = false;
					m_jumpTimer = 0.0f;
					break;
				}

				enemy.first->CallInteract(collisionFlag);
			}
		}
	}
}

void MyGame::Player::OnlyCheckStaticCollision()
{
	int playerMapXidx = (int)(m_posX < 0.0f ? m_posX - TILE_SPR_SIZE_X : m_posX) / TILE_SPR_SIZE_X;
	int playerMapYidx = (int)(m_posY < 0.0f ? m_posY - TILE_SPR_SIZE_Y : m_posY) / TILE_SPR_SIZE_Y;

	for (int i = playerMapYidx; i <= playerMapYidx + 2; i++)
		for (int j = playerMapXidx; j <= playerMapXidx + 2; j++)
		{
			if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
				continue;

			float p_minX = m_posX;
			float p_maxX = m_posX + PLAYER_SPR_SIZE_X;
			float p_minY = m_posY;
			float p_maxY = m_posY + PLAYER_SPR_SIZE_Y;

			float t_minX = j * TILE_SPR_SIZE_X;
			float t_maxX = (j + 1) * TILE_SPR_SIZE_X;
			float t_minY = i * TILE_SPR_SIZE_Y;
			float t_maxY = (i + 1) * TILE_SPR_SIZE_Y;

			int collisionFlag = 0;

			if (m_scene->CheckTileCollision(m_scene->GetTilemap()[i][j])
				&& MyGame::CheckAABB(p_maxX, p_maxY, p_minX, p_minY,
					t_maxX, t_maxY, t_minX, t_minY))
			{
				collisionFlag = ApplyPenetration(&m_posX, &m_posY, p_maxX, p_maxY, p_minX, p_minY,
					t_maxX, t_maxY, t_minX, t_minY);
			}

			if (collisionFlag == 1 || collisionFlag == 2)
			{
				if (p_maxY - 0.5 >= t_minY
					&& p_minY + 0.5 < t_maxY)
					m_velX = 0.0f;
			}
			if (collisionFlag == 4)
			{
				m_jumpTrigger = false;
				m_jumpTimer = 0.0f;
			}
		}
}

void MyGame::Player::ForceChangePlayerShape(int idx)
{
	switch (idx)
	{
	case 0:
		m_spr.ShapeString = m_playerShape1;
		break;
	case 1:
		m_spr.ShapeString = m_playerShape2;
		break;
	case 2:
		m_spr.ShapeString = m_playerShape3;
		break;
	case 3:
		m_spr.ShapeString = m_playerGrabShape;
		break;
	case 4:
		m_spr.ShapeString = m_playerDieShape;
		break;
	}
}

