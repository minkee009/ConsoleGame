#include "DeathShroom.hpp"
#include "engine.hpp"
#include "math.hpp"

MyGame::DeathShroom::DeathShroom(PlayScene* scene)
{
	m_scene = scene;

	m_spr =
	{
		{SHROOM_SPR_SIZE_X,SHROOM_SPR_SIZE_Y},
		{ 0, 0 },
		m_shroomShape1,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};
}

void MyGame::DeathShroom::Initialize()
{
	m_isAlive = true;
	m_posX = m_spawnPosX;
	m_posY = m_spawnPosY;
	m_velX = 20.0f;
	m_spr.ShapeString = m_shroomShape1;
}

void MyGame::DeathShroom::Update()
{

	int mapXIdx = (int)(m_posX < 0.0f ? m_posX - TILE_SPR_SIZE_X : m_posX) / TILE_SPR_SIZE_X;
	int mapYIdx = (int)(m_posY < 0.0f ? m_posY - TILE_SPR_SIZE_Y : m_posY) / TILE_SPR_SIZE_Y;

	auto lastGrounded = m_isGrounded;

	m_isGrounded = false;
	if (m_velY >= -0.001f)
	{
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

		for (int i = mapYIdx + 1; i <= mapYIdx + 2; i++)
			for (int j = mapXIdx; j <= mapXIdx + 2; j++)
			{
				if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
					continue;

				auto pickY = (i)*TILE_SPR_SIZE_Y - 0.02f;//타일의 y좌표
				auto pick = m_scene->GetTilemap()[i][j];	 //현재 타일
				auto pickXMin = j * TILE_SPR_SIZE_X;        //타일의 x좌표 최소값
				auto pickXMax = (j + 1) * TILE_SPR_SIZE_X;  //타일의 x좌표 최대값

				if ((m_scene->CheckTileCollision(pick))
					&& ((pickXMax > m_posX && pickXMin < m_posX + SHROOM_SPR_SIZE_X)
						|| (pickXMin < m_posX + SHROOM_SPR_SIZE_X && pickXMax >= m_posX))
					&& pickY < m_posY + SHROOM_SPR_SIZE_Y)
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

	m_posX += m_velX * GET_DELTATIME();
	m_posY += m_velY * GET_DELTATIME();
}

void MyGame::DeathShroom::CheckCollision()
{
	int goombaMapXidx = (int)(m_posX < 0.0f ? m_posX - TILE_SPR_SIZE_X : m_posX) / TILE_SPR_SIZE_X;
	int goombaMapYidx = (int)(m_posY < 0.0f ? m_posY - TILE_SPR_SIZE_Y : m_posY) / TILE_SPR_SIZE_Y;

	for (int i = goombaMapYidx; i <= goombaMapYidx + 2; i++)
		for (int j = goombaMapXidx; j <= goombaMapXidx + 2; j++)
		{
			if (i > MAP01_SIZE_Y - 1 || i < 0 || j > MAP01_SIZE_X - 1 || j < 0)
				continue;

			float p_minX = m_posX;
			float p_maxX = m_posX + SHROOM_SPR_SIZE_X;
			float p_minY = m_posY;
			float p_maxY = m_posY + SHROOM_SPR_SIZE_Y;

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
			int collisionFlag = ApplyPenetration(&m_posX, &m_posY, GetBbox(), tile.first->GetBbox());

			if ((MATH_COL_FLAG_PUSHLEFT | MATH_COL_FLAG_PUSHRIGHT) & collisionFlag)
			{
				m_velX *= -1.0f;
			}
		}
	}

	//적 충돌
	for (auto& enemy : *(m_scene->GetEnemys()))
	{
		if (!enemy.first->GetActive()
			|| enemy.first->GetForceIgnoreCollision()
			|| enemy.first->IsActtacked()
			|| !enemy.first->IsAlive())
			continue;

		//경계범위 체크
		bool isCollide = CheckAABB(GetBbox(),
			enemy.first->GetBbox());

		if (isCollide)
		{
			auto collisionFlag = CalcPenetration(GetBbox(),
				enemy.first->GetBbox());

			enemy.first->CallInteract(MATH_COL_FLAG_PUSHDOWN);

			m_isAlive = false;
		}
	}
}

void MyGame::DeathShroom::CallInteract(int collisionFlag, int whoFlag)
{
	if (whoFlag & IS_PLAYER)
	{
		m_scene->gameState = PlayerDead;
	}

	if (whoFlag & IS_TILE && collisionFlag & MATH_COL_FLAG_PUSHDOWN)
	{
		m_velY -= PLAYER_JUMPVEL;
	}
}
