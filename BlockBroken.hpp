#pragma once
#include "Tile.hpp"

#define BLOCK_BROKEN_SPR_SIZE_X 2
#define BLOCK_BROKEN_SPR_SIZE_Y 2

namespace MyGame
{
	class BlockBroken : public Tile
	{
	public:
		BlockBroken(PlayScene* scene);

		void SetVelocity(float x, float y) { m_velX = x; m_velY = y; }
		void Initialize() override;
		void Update() override;
		void CallInteract(int collisionFlag) override;
	private:
		static constexpr const WCHAR* m_blockShape[BLOCK_BROKEN_SPR_SIZE_Y] = {
			L"⣹⣿",
			L"⣭⠏",
		};

		float m_velX;
		float m_velY;
	};
}