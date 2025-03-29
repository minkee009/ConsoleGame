#pragma once
#include "Tile.hpp"

#define GOAL_FLAG_SPR_SIZE_X 8
#define GOAL_FLAG_SPR_SIZE_Y 3

namespace MyGame
{
	class GoalFlag : public Tile
	{
	public:
		GoalFlag(PlayScene* scene);

		void Initialize() override;
		void Update() override;
	private:
		static constexpr const WCHAR* m_goalFlagShape[GOAL_FLAG_SPR_SIZE_Y] = {
		L"████",
		L"████████",
		L"████",
		};
	};
}