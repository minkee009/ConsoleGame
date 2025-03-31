#pragma once
#include "PlayScene.hpp"
#include "Item.hpp"

#define SHROOM_SPR_SIZE_X 6
#define SHROOM_SPR_SIZE_Y 3

namespace MyGame
{
	class DeathShroom : public Item
	{
	public:
		DeathShroom(PlayScene* scene);

		void Initialize();
		void Update() override;
		void CheckCollision() override;
		void CallInteract(int collisionFlag, int otherFlag) override;
		Item* Clone() const override { return new DeathShroom(*this); }
	private:
		static constexpr const WCHAR* m_shroomShape1[SHROOM_SPR_SIZE_Y] = {
			L"⠀⠠⣄⡶⢄⠀",
			L"⠀⠓⣿⣗⠍⠀",
			L"⠀⠀⠻⠿⠁⠀",
		};

		bool m_isGrounded;
	};
}