#include "GoomBa.hpp"
#include "engine.hpp"
#include "math.hpp"

MyGame::GoomBa::GoomBa(PlayScene* scene)
{
	m_scene = scene;

	m_spr = 
	{
		{GOOMBA_SPR_SIZE_X,GOOMBA_SPR_SIZE_Y},
		{ 0, 0 },
		m_goombaShape1,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};
}

void MyGame::GoomBa::Initialize()
{
	m_isAlive = true;
	m_pressed = false;
	m_attacked = false;
	m_dontPress = false;
	m_posX = m_spawnPosX;
	m_posY = m_spawnPosY;
	m_velX = -12.0f;
	m_timer = 0.0f;
	m_spr.ShapeString = m_goombaShape1;
}

void MyGame::GoomBa::Update()
{
	if (!m_active || !m_isAlive) return;

	if (m_pressed)
	{
		m_timer += GET_DELTATIME();
		if (m_timer > 0.3f)
		{
			m_isAlive = false;
		}
		return;
	}

	int goombaMapXidx = (int)(m_posX < 0.0f ? m_posX - TILE_SPR_SIZE_X : m_posX) / TILE_SPR_SIZE_X;
	int goombaMapYidx = (int)(m_posY < 0.0f ? m_posY - TILE_SPR_SIZE_Y : m_posY) / TILE_SPR_SIZE_Y;

	auto lastGrounded = m_isGrounded;

	m_isGrounded = false;
	if (m_velY >= -0.001f)
		for (int i = goombaMapYidx + 1; i <= goombaMapYidx + 2; i++)
			for (int j = goombaMapXidx; j <= goombaMapXidx + 2; j++)
			{
				if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
					continue;

				auto pickY = (i)*TILE_SPR_SIZE_Y - 0.02f;//타일의 y좌표
				auto pick = m_scene->GetTilemap()[i][j];	 //현재 타일
				auto pickXMin = j * TILE_SPR_SIZE_X;        //타일의 x좌표 최소값
				auto pickXMax = (j + 1) * TILE_SPR_SIZE_X;  //타일의 x좌표 최대값

				if ((m_scene->CheckTileCollision(pick))
					&& ((pickXMax > m_posX && pickXMin < m_posX + GOOMBA_SPR_SIZE_X)
						|| (pickXMin < m_posX + GOOMBA_SPR_SIZE_X && pickXMax >= m_posX))
					&& pickY < m_posY + GOOMBA_SPR_SIZE_Y)
				{
					m_isGrounded = true;
					m_velY = 0.0f;
					break;
				}
			}


	if (!m_isGrounded)
	{
		m_velY += PLAYER_GRAVITY * GET_DELTATIME();
		m_velY = min(PLAYER_MAXFALLSPEED, m_velY);
	}

	m_posX += m_velX * GET_DELTATIME();
	m_posY += m_velY * GET_DELTATIME();
	m_timer += GET_DELTATIME();

	if (m_timer > 0.325f)
	{
		m_timer = 0.0f;
		m_sprIdx++;
		m_sprIdx %= GOOMBA_SPR_ANIM_MAXFRAME;

		switch (m_sprIdx)
		{
		case 0:
			m_spr.ShapeString = m_goombaShape1;
			break;
		case 1:
			m_spr.ShapeString = m_goombaShape2;
			break;
		}
	}
}

void MyGame::GoomBa::CheckCollision()
{
	int goombaMapXidx = (int)(m_posX < 0.0f ? m_posX - TILE_SPR_SIZE_X : m_posX) / TILE_SPR_SIZE_X;
	int goombaMapYidx = (int)(m_posY < 0.0f ? m_posY - TILE_SPR_SIZE_Y : m_posY) / TILE_SPR_SIZE_Y;

	for (int i = goombaMapYidx; i <= goombaMapYidx + 2; i++)
		for (int j = goombaMapXidx; j <= goombaMapXidx + 2; j++)
		{
			if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
				continue;

			float p_minX = m_posX;
			float p_maxX = m_posX + GOOMBA_SPR_SIZE_X;
			float p_minY = m_posY;
			float p_maxY = m_posY + GOOMBA_SPR_SIZE_Y;

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

			if ((MATH_COL_FLAG_PUSHLEFT | MATH_COL_FLAG_PUSHRIGHT) & collisionFlag)
			{
				m_velX *= -1.0f;
			}
		}
}

void MyGame::GoomBa::CallInteract(int collisionFlag)
{
	if (collisionFlag & MATH_COL_FLAG_PUSHUP)
	{
		m_timer = 0.0f;
		m_pressed = true;
		m_attacked = true;
		m_spr.ShapeString = m_goombaShapePress;
		m_scene->PrintPoint(L"100",m_posX,m_posY);
		m_scene->AddScore(100.0f);
	}
}
