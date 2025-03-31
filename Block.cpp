#include "Block.hpp"
#include "engine.hpp"
#include "Player.hpp"
#include "math.hpp"
#include "BlockBroken.hpp"

MyGame::Block::Block(PlayScene* scene)
{
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

void MyGame::Block::Initialize()
{
	m_isDeleted = false;
}

void MyGame::Block::Update()
{

}

void MyGame::Block::CallInteract(int collisionFlag)
{
	if (collisionFlag & MATH_COL_FLAG_PUSHDOWN)
	{
		m_isDeleted = true;

		BlockBroken* broken = new BlockBroken(m_scene);
		broken->SetVelocity(10.0f,-20.0f);
		broken->SetPosition(m_posX + 1.0f, m_posY);
		m_scene->AddTile(broken);

		broken = new BlockBroken(m_scene);
		broken->SetVelocity(-10.0f, -20.0f);
		broken->SetPosition(m_posX - 1.0f, m_posY);
		m_scene->AddTile(broken);

		Bbox hitUp = GetBbox();
		hitUp.minY -= 0.5f;
		hitUp.maxY -= 0.5f;

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
