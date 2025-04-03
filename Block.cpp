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
	//�÷��̾� �� �ϵ��ڵ�
	if (collisionFlag & MATH_COL_FLAG_PUSHDOWN
		&& m_scene->GetPlayer()->GetVelY() < 0.0f)
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

		PLAY_SE("SE\\brockbreak.mp3",1);

		//������� ������Ʈ�� ��ȣ�ۿ�

		//������
		for (auto& item : *m_scene->GetItems())
		{
			if (!item.first->GetActive() || item.first->IsInstancePlay())
				continue;

			if (CheckAABB(hitUp, item.first->GetBbox()))
			{
				item.first->CallInteract(MATH_COL_FLAG_PUSHDOWN, IS_TILE);
			}
		}

		//��
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
