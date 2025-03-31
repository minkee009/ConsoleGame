#pragma once
#include "PlayScene.hpp"
#include "Enemy.hpp"
#include "NinjaStar.hpp"

#define NINJA_SPR_SIZE_X 6
#define NINJA_SPR_SIZE_Y 5

#define NINJA_SPR_ANIM_MAXFRAME 2
#define NINJA_MAX_RANDACT 17

namespace MyGame
{
	class Ninja : public Enemy
	{
	public:
		Ninja(PlayScene* scene);

		void Initialize() override;
		void Update() override;
		void CheckCollision() override;
		void CallInteract(int collisionFlag) override;
		Enemy* Clone() const override { return new Ninja(*this); }
	private:
		static constexpr const WCHAR* m_ninjaShape1[NINJA_SPR_SIZE_Y] = {
			L"^--^",
			L"(===)",
			L"|#  |",
			L"|___|",
			L")   ) ",
		};
		static constexpr const WCHAR* m_ninjaShape2[NINJA_SPR_SIZE_Y] = {
			L"^--^",
			L"(===)",
			L"#   |",
			L"|___|",
			L"  )) ",
		};
		static constexpr const WCHAR* m_ninjaShapeDash[NINJA_SPR_SIZE_Y] = {
			L"=====",
			L")))))",
			L"##>>>",
			L">>>>>",
			L"///  ",
		};
		static constexpr const WCHAR* m_ninjaShapePress[NINJA_SPR_SIZE_Y] = {
			L"     ",
			L"     ",
			L"_____",
			L"(XoX)",
			L" > > ",
		};

		static constexpr const WCHAR* m_ninjaShapeShoot[NINJA_SPR_SIZE_Y] = {
			L"^--^",
			L"(===)",
			L"|  --",
			L"|___|",
			L"  )) ",
		};

		static constexpr const WCHAR* m_ninjaShapeDead[NINJA_SPR_SIZE_Y] = {
			L")   )",
			L"|---|",
			L"|   #",
			L"(===)",
			L" V V ",
		};

		float m_timer;
		float m_moveTimer;
		float m_actTimer;

		NinjaStar* m_ninjaStar;

		short m_actIdx;
		short m_randActIdx;
		const bool m_randAct[NINJA_MAX_RANDACT] = { false,true,true,false,true,true,false,true,false,true,false,false,true,false,false,false };
		short m_sprIdx;
		bool m_isGrounded;
		bool m_pressed;
		bool m_smashed;
	};
}