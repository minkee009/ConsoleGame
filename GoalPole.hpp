#pragma once
#include "PlayScene.hpp"
#include "Tile.hpp"

#define GOAL_POLE_SPR_SIZE_X 2
#define GOAL_POLE_SPR_SIZE_Y 26

namespace MyGame
{
	class GoalPole : public Tile
	{
	public:
		GoalPole(PlayScene* scene);
		//void Initialize() override;
		//void Update() override;
		void CallInteract(int collisionFlag) override;
	private:
		static constexpr const WCHAR* m_goalPoleShape[GOAL_POLE_SPR_SIZE_Y] = {
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		};
	};
}