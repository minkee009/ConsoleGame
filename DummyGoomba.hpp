#pragma once
//푹신푹신한 굼바, 밟아도 죽지않으며 동시에 플레이어를 높게 점프시킨다
#pragma once
#include "PlayScene.hpp"
#include "Enemy.hpp"

#define GOOMBA_SPR_SIZE_X 5
#define GOOMBA_SPR_SIZE_Y 2

#define GOOMBA_SPR_ANIM_MAXFRAME 2

namespace MyGame
{
	class DummyGoomBa : public Enemy
	{
	public:
		DummyGoomBa(PlayScene* scene);

		void Initialize() override;
		void Update() override;
		void CheckCollision() override;
		void CallInteract(int collisionFlag) override;
		Enemy* Clone() const override { return new DummyGoomBa(*this); }
	private:
		static constexpr const WCHAR* m_goombaShape1[GOOMBA_SPR_SIZE_Y] = {
			L"(@ @)",
			L"(###)",
		};
		static constexpr const WCHAR* m_goombaShape2[GOOMBA_SPR_SIZE_Y] = {
			L"(@ @)",
			L"(---)",
		};
		static constexpr const WCHAR* m_goombaShapePress[GOOMBA_SPR_SIZE_Y] = {
			L" ___ ",
			L"(@#@)",
		};

		static constexpr const WCHAR* m_goombaShapeDead[GOOMBA_SPR_SIZE_Y] = {
			L"(---)",
			L"(x x)",
		};

		float m_timer;
		short m_sprIdx;
		bool m_isGrounded;
		bool m_pressed;
		bool m_smashed;
	};
}