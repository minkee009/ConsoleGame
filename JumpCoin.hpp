#pragma once
#include "PlayScene.hpp"
#include "Item.hpp"

#define JUMPCOIN_SPR_SIZE_X 1
#define JUMPCOIN_SPR_SIZE_Y 1

#define JUMPCOIN_SPR_ANIM_MAXFRAME 2

namespace MyGame
{
	class JumpCoin : public Item
	{
	public:
		JumpCoin(PlayScene* scene);

		void Initialize() override;
		void Update() override;
		void CheckCollision() override;
		void CallInteract(int collisionFlag, int otherFlag) override;
		Item* Clone() const override { return new JumpCoin(*this); }
	private:
		static constexpr const WCHAR* m_jumpCoinShape1[JUMPCOIN_SPR_SIZE_Y] = {
			L"@",
		};
		static constexpr const WCHAR* m_jumpCoinShape2[JUMPCOIN_SPR_SIZE_Y] = {
			L"|",
		};

		float m_instanceTimer;
		float m_timer;
		short m_sprIdx;
	};
}