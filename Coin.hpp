#pragma once
#include "PlayScene.hpp"
#include "Item.hpp"

#define COIN_SPR_SIZE_X 3
#define COIN_SPR_SIZE_Y 3

#define COIN_SPR_ANIM_MAXFRAME 2

namespace MyGame
{
	class Coin : public Item
	{
	public:
		Coin(PlayScene* scene);

		void Initialize();
		void Update() override;
		void CallInteract(int collisionFlag, int otherFlag) override;
		Item* Clone() const override { return new Coin(*this); }
	private:
		static constexpr const WCHAR* m_coinShape1[COIN_SPR_SIZE_Y] = {
			L" O ",
			L"O O",
			L" O",
		};
		static constexpr const WCHAR* m_coinShape2[COIN_SPR_SIZE_Y] = {
			L" #",
			L"###",
			L" # ",
		};

		float m_timer;
		short m_sprIdx;
	};
}