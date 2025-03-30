#include "Block.hpp"
#include "engine.hpp"
#include "Player.hpp"

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
}