#pragma once
#include "PlayScene.hpp"
#include "Item.hpp"

#define COIN_SPR_SIZE_X 4
#define COIN_SPR_SIZE_Y 4

#define COIN_SPR_ANIM_MAXFRAME 2

namespace MyGame
{
	class Coin : public Item
	{
	public:
		Coin(PlayScene* scene);

		void Initialize() override;
		void Update() override;
		void CheckCollision() override;
		void CallInteract(int collisionFlag, int otherFlag) override;
		Item* Clone() const override { return new Coin(*this); }
	private:
		static constexpr const WCHAR* m_coinShape1[COIN_SPR_SIZE_Y] = {
			L" OO ",
			L"O  O",
			L"O OO",
			L" OO ",
		};
		static constexpr const WCHAR* m_coinShape2[COIN_SPR_SIZE_Y] = {
			L" ## ",
			L"#  #",
			L"# ##",
			L" ## ",
		};

		float m_instanceTimer;
		float m_timer;
		short m_sprIdx;
	};
}