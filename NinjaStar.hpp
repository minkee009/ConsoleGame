#pragma once
#include "Tile.hpp"

#define NINJASTAR_SPR_SIZE_X 8
#define NINJASTAR_SPR_SIZE_Y 1
#define NINJASTAR_SPEED 100

namespace MyGame
{
	class NinjaStar : public Tile
	{
	public:
		NinjaStar(PlayScene* scene);

		void Initialize() override;
		void Update() override;
		void CallInteract(int collisionFlag) override;
		void SetDirAndPos(bool dir, float posX, float posY) { m_dir = dir; m_spr.Flip = !dir; m_posX = posX, m_posY = posY; }
	private:
		static constexpr const WCHAR* m_goalFlagShape[NINJASTAR_SPR_SIZE_Y] = {
		L"##-     ",
		};

		bool m_dir;
	};
}