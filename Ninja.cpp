#include "Ninja.hpp"
#include "engine.hpp"
#include "math.hpp"

MyGame::Ninja::Ninja(PlayScene* scene)
{
	m_scene = scene;
	m_randActIdx = 0;
	m_spr =
	{
		{NINJA_SPR_SIZE_X,NINJA_SPR_SIZE_Y},
		{ 0, 0 },
		m_ninjaShape1,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		true
	};
}

void MyGame::Ninja::Initialize()
{
	m_isAlive = true;
	m_pressed = false;
	m_attacked = false;
	m_smashed = false;
	m_dontPress = false;
	m_posX = m_spawnPosX;
	m_posY = m_spawnPosY;
	m_velX = -8.0f;
	m_velY = 0.0f;
	m_timer = 0.0f;
	m_actTimer = 0;
	m_actIdx = 0;
	m_sprIdx = 0;
	m_spr.ShapeString = m_ninjaShape1;

	m_ninjaStar = nullptr;
	m_ninjaStar = new NinjaStar(m_scene);
	m_scene->AddTile(m_ninjaStar);
}

void MyGame::Ninja::Update()
{
	//if (!m_active || !m_isAlive) return;

	if (m_pressed)
	{
		m_timer += GET_DELTATIME();
		if (m_timer > 0.3f)
		{
			m_isAlive = false;
		}
		return;
	}

	if (m_smashed)
	{
		m_velY += PLAYER_GRAVITY * GET_DELTATIME();
		m_velY = min(PLAYER_MAXFALLSPEED, m_velY);
		m_posX += m_velX * GET_DELTATIME();
		m_posY += m_velY * GET_DELTATIME();
		return;
	}


	int goombaMapXidx = (int)(m_posX < 0.0f ? m_posX - TILE_SPR_SIZE_X : m_posX) / TILE_SPR_SIZE_X;
	int goombaMapYidx = (int)(m_posY < 0.0f ? m_posY - TILE_SPR_SIZE_Y : m_posY) / TILE_SPR_SIZE_Y;

	auto lastGrounded = m_isGrounded;

	m_isGrounded = false;
	if (m_velY >= -0.001f)
	{
		for (int i = goombaMapYidx; i <= goombaMapYidx + 2; i++)
			for (int j = goombaMapXidx; j <= goombaMapXidx + 2; j++)
			{
				if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
					continue;

				auto pickY = (i)*TILE_SPR_SIZE_Y - 0.02f;//타일의 y좌표
				auto pick = m_scene->GetTilemap()[i][j];	 //현재 타일
				auto pickXMin = j * TILE_SPR_SIZE_X;        //타일의 x좌표 최소값
				auto pickXMax = (j + 1) * TILE_SPR_SIZE_X;  //타일의 x좌표 최대값

				if ((m_scene->CheckTileCollision(pick))
					&& ((pickXMax > m_posX && pickXMin < m_posX + NINJA_SPR_SIZE_X)
						|| (pickXMin < m_posX + NINJA_SPR_SIZE_X && pickXMax >= m_posX))
					&& pickY < m_posY + NINJA_SPR_SIZE_Y)
				{
					m_isGrounded = true;
					m_velY = 0.0f;
					break;
				}
			}

		Bbox footbox = GetBbox();
		footbox.maxY += 0.02f;

		for (auto tile : *(m_scene->GetTiles()))
		{
			if (tile.first->GetActive()
				&& tile.first->IsSolid()
				&& !tile.first->IsOnlyDownHit()
				&& CheckAABB(footbox,
					tile.first->GetBbox())
				&& MATH_COL_FLAG_PUSHUP & CalcPenetration(footbox, tile.first->GetBbox()))
			{
				m_isGrounded = true;
				m_velY = 0.0f;
				break;
			}
		}
	}
		
	if (!m_isGrounded)
	{
		m_velY += PLAYER_GRAVITY * GET_DELTATIME();
		m_velY = min(PLAYER_MAXFALLSPEED, m_velY);
	}

	switch (m_actIdx)
	{
	case 0:
	case 2:
	{
		m_dontPress = false;
		m_timer += GET_DELTATIME();

		if (m_timer > 0.125f)
		{
			m_timer = 0.0f;
			m_sprIdx++;
			m_sprIdx %= NINJA_SPR_ANIM_MAXFRAME;

			switch (m_sprIdx)
			{
			case 0:
				m_spr.ShapeString = m_ninjaShape1;
				break;
			case 1:
				m_spr.ShapeString = m_ninjaShape2;
				break;
			}
		}

		bool dir = m_randAct[m_randActIdx];

		m_spr.Flip = m_scene->GetPlayer()->GetPosX() > m_posX ? false : true;
		m_velX = (dir ? 1 : -1) * 20.0f;
		m_actTimer += GET_DELTATIME();
		if (m_actTimer > 0.7f)
		{
			m_actIdx++;
			m_actTimer = 0.0f;

			m_randActIdx++;
			m_randActIdx %= NINJA_MAX_RANDACT;
			break;
		}

		break;
	}
	case 1:
	case 3:
	{
		m_spr.Flip = m_scene->GetPlayer()->GetPosX() > m_posX ? false : true;
		m_velX = 0.0f;
		m_actTimer += GET_DELTATIME();
		if (m_actTimer > 0.35f)
		{
			m_actTimer = 0.0f;
			if (m_actIdx == 3)
			{
				m_actIdx = m_randAct[m_randActIdx++] ? 4 : 5;
				m_randActIdx %= NINJA_MAX_RANDACT;
				if (m_actIdx == 5)
				{
					m_spr.Flip = m_scene->GetPlayer()->GetPosX() > m_posX ? false : true;
					m_ninjaStar->SetActive(true);
					m_ninjaStar->SetDirAndPos(m_spr.Flip, m_posX + (NINJA_SPR_SIZE_X * 0.5f), m_posY + 2.0f);
				}
					break;
			}
			else
				m_actIdx++;
		}
		break;
	}
	case 4:
	{
		m_actTimer += GET_DELTATIME();
		bool dir = m_randAct[m_randActIdx];
		m_spr.Flip = dir ? false : true;
		m_dontPress = true;
		m_velX = (dir ? 1 : -1) * 120.0f;
		m_spr.ShapeString = m_ninjaShapeDash;
		if (m_actTimer > 0.2f)
		{
			m_actIdx = 0;
			m_actTimer = 0.0f;
			m_spr.ShapeString = m_ninjaShape1;
			break;
		}
		break;
	}
	case 5:
	{
		m_spr.Flip = m_scene->GetPlayer()->GetPosX() > m_posX ? false : true;
		m_actTimer += GET_DELTATIME();
		m_spr.ShapeString = m_ninjaShapeShoot;
		if (m_actTimer > 1.2f)
		{
			m_actIdx = 0;
			m_actTimer = 0.0f;
			m_spr.ShapeString = m_ninjaShape1;
			m_randActIdx++;
			m_randActIdx %= NINJA_MAX_RANDACT;
			break;
		}
		break;
	}
	}

	m_posX += m_velX * GET_DELTATIME();
	m_posY += m_velY * GET_DELTATIME();

}

void MyGame::Ninja::CheckCollision()
{
	int goombaMapXidx = (int)(m_posX < 0.0f ? m_posX - TILE_SPR_SIZE_X : m_posX) / TILE_SPR_SIZE_X;
	int goombaMapYidx = (int)(m_posY < 0.0f ? m_posY - TILE_SPR_SIZE_Y : m_posY) / TILE_SPR_SIZE_Y;

	for (int i = goombaMapYidx - 1; i <= goombaMapYidx + 4; i++)
		for (int j = goombaMapXidx; j <= goombaMapXidx + 2; j++)
		{
			if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
				continue;

			float p_minX = m_posX;
			float p_maxX = m_posX + NINJA_SPR_SIZE_X;
			float p_minY = m_posY;
			float p_maxY = m_posY + NINJA_SPR_SIZE_Y;

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
		}

	//타일 충돌
	for (auto& tile : *(m_scene->GetTiles()))
	{
		if (!tile.first->GetActive() || tile.first->IsOnlyDownHit() || !tile.first->IsSolid())
			continue;

		//경계범위 체크
		bool isCollide = CheckAABB(GetBbox(),
			tile.first->GetBbox());

		if (isCollide)
		{
			ApplyPenetration(&m_posX, &m_posY, GetBbox(), tile.first->GetBbox());
		}
	}
}

void MyGame::Ninja::CallInteract(int collisionFlag)
{
	if (collisionFlag & MATH_COL_FLAG_PUSHUP)
	{
		if (m_dontPress)
		{
			m_scene->gameState = PlayerDead;
		}
		else
		{
			m_timer = 0.0f;
			m_pressed = true;
			m_attacked = true;
			m_spr.ShapeString = m_ninjaShapePress;
			m_scene->PrintPoint(L"400", m_posX, m_posY);
			m_scene->AddScore(400.0f);
		}
	}

	else if (collisionFlag & MATH_COL_FLAG_PUSHDOWN)
	{
		m_timer = 0.0f;
		m_attacked = true;
		m_smashed = true;
		m_spr.ShapeString = m_ninjaShapeDead;
		m_scene->PrintPoint(L"400", m_posX, m_posY);
		m_scene->AddScore(400.0f);
		m_velY = -25.0f;
	}
}
