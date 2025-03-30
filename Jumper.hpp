#pragma once
#include "PlayScene.hpp"
#include "Enemy.hpp"

#define JUMPER_SPR_SIZE_X 5
#define JUMPER_SPR_SIZE_Y 10

#define JUMPER_SPR_ANIM_MAXFRAME 2

namespace MyGame
{
	class Jumper : public Enemy
	{
	public:
		Jumper(PlayScene* scene);

		void Initialize() override;
		void Update() override;
		void CheckCollision() override;
		void CallInteract(int collisionFlag) override;
	private:
		static constexpr const WCHAR* m_jumperShape1[JUMPER_SPR_SIZE_Y] = {
			L"^--^",
			L"(-.-)",
			L"|U U|",
			L"|   |",
			L"v__v ",
			L" ^--^",
			L"(T.T)",
			L"I| |I",
			L" | | ",
			L" > > ",
		};
		static constexpr const WCHAR* m_jumperShape2[JUMPER_SPR_SIZE_Y] = {
			L"^--^",
			L"(-.-)",
			L"|U U|",
			L"|   |",
			L"v__v ",
			L" ^--^",
			L"(T.T)",
			L"I| |I",
			L" | | ",
			L"  >. ",
		};
		static constexpr const WCHAR* m_jumperShapeJump[JUMPER_SPR_SIZE_Y] = {
			L"^--^",
			L"(-.-)",
			L"|U U|",
			L"|   |",
			L"v__v ",
			L" ^--^",
			L"(T.T)",
			L"I| |I",
			L" | | ",
			L" \\ /",
		};
		static constexpr const WCHAR* m_jumperShapePress[JUMPER_SPR_SIZE_Y] = {
			L"\0\0\0\0\0",
			L"\0\0\0\0\0",
			L"\0\0\0\0\0",
			L"\0\0\0\0\0",
			L"\0\0\0\0\0",
			L"\0\0\0\0\0",
			L" ___",
			L"(-.-)",
			L" ___",
			L"(T.T)",
		};

		static constexpr const WCHAR* m_jumperShapeDead[JUMPER_SPR_SIZE_Y] = {
			L" > > ",
			L" | | ",
			L"I| |I",
			L"(x.x)",
			L"v--v "
			L"^--^",
			L"|   |",
			L"|U U|",
			L"(X.X)",
			L"v--v ",

		};

		float m_timer;
		float m_jumpTimer;
		short m_sprIdx;
		bool m_isGrounded;
		bool m_pressed;
	};
}