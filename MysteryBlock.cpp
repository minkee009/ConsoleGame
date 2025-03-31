#include "MysteryBlock.hpp"
#include "engine.hpp"
#include "Player.hpp"
#include "math.hpp"
#include "BlockBroken.hpp"

MyGame::MysteryBlock::MysteryBlock(PlayScene* scene)
{
	m_enemy = nullptr;
	m_scene = scene;

	m_spr =
	{
		{ BLOCK_SPR_SIZE_X, BLOCK_SPR_SIZE_Y },
		{ 0, 0 },
		m_blockShape,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		false
	};
	m_isSolid = true;
}

void MyGame::MysteryBlock::Initialize()
{
	m_prevEnemy = nullptr;
	m_isHit = false;
	m_spr.ShapeString = m_blockShape;
	m_timer = 0.0f;
	//m_created = false;
}

void MyGame::MysteryBlock::Update()
{
	if (m_isHit)// && !m_created)
	{
		if (m_timer > 0.3f)
		{
			m_prevEnemy->SetForceIgnoreCollision(false);
			//m_created = true;
			return;
		}
		m_timer += GET_DELTATIME();
		m_prevEnemy->SetPosition(m_prevEnemy->GetPosX(), lerp_s_f(m_posY, m_posY - m_prevEnemy->GetSprite()->Size.Y, m_timer / 0.3f));
		
	}
}

void MyGame::MysteryBlock::CallInteract(int collisionFlag)
{
	if (m_isHit)
		return;

	if (collisionFlag & MATH_COL_FLAG_PUSHDOWN)
	{
		m_isHit = true;
		m_spr.ShapeString = m_blockHitShape;

		Bbox hitUp = GetBbox();
		hitUp.minY -= 0.5f;
		hitUp.maxY -= 0.5f;

		//미스터리 블록에 무엇이 들어가있는지 확인

		//미스터리 블록의 아이템넣기는 무조건 씬에서 처리됨이 보장됨
		if (m_enemy != nullptr)
		{
			m_enemy->SetForceIgnoreCollision(true);
			m_enemy->SetPosition(m_posX, m_posY);
			m_enemy->SetSpawnPos(m_posX, m_posY);
			m_enemy->Initialize();
			m_scene->AddEnemy(m_enemy);
			//복사 생성자 호출
			//씬으로 옮긴 객체는 씬이 자동으로 
			m_prevEnemy = m_enemy;
			m_enemy = m_enemy->Clone();
		}


		//블록위의 오브젝트와 상호작용

		//아이템

		//적
		for (auto& enemy : *m_scene->GetEnemys())
		{
			if (!enemy.first->GetActive())
				continue;

			if (CheckAABB(hitUp, enemy.first->GetBbox()))
			{
				enemy.first->CallInteract(MATH_COL_FLAG_PUSHDOWN);
			}
		}
	}
}
