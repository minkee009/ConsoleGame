#pragma once
#include "Tile.hpp"

#define BLOCK_SPR_SIZE_X 6
#define BLOCK_SPR_SIZE_Y 3

namespace MyGame
{
	class Block : public Tile
	{
	public:
		Block(PlayScene* scene);

		void Initialize() override;
		void Update() override;
		void CallInteract(int collisionFlag) override;
	private:
		static constexpr const WCHAR* m_blockShape[BLOCK_SPR_SIZE_Y] = {
			L"⣿⣿⣿⣭⣭⣭",
			L"⣭⣭⣭⣿⣿⣿",
			L"⣿⣿⣿⣭⣭⣭",
		};
	};
}