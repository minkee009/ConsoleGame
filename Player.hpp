#pragma once
#include "PlayScene.hpp"

#define PLAYER_SPR_SIZE_X 6
#define PLAYER_SPR_SIZE_Y 3
#define PLAYER_SPR_PIVOT_X 0
#define PLAYER_SPR_PIVOT_Y 0

#define PLAYER_SPR_ANIM_MAXFRAME 2
#define PLAYER_STEPRATE 0.3

#define PLAYER_ACCEL 35
#define PLAYER_MAXSPEED 24
#define PLAYER_DECEL 40

#define PLAYER_AIR_ACCEL 35
#define PLAYER_GRAVITY 150
#define PLAYER_MAXFALLSPEED 50
#define PLAYER_JUMPVEL 30
#define PLAYER_JUMPVEL_MAX 32
#define PLAYER_JUMPTIME 0.3f

#define PLAYER_SPR_S_WALK1 0
#define PLAYER_SPR_S_WALK2 1
#define PLAYER_SPR_S_JUMP 2
#define PLAYER_SPR_S_GRAB 3
#define PLAYER_SPR_S_DIE 4

namespace MyGame
{
	class PlayScene;
	class Player
	{
	public:
		Player(PlayScene* scene);
		~Player();

		void Initialize();

		void SetPosition(float x, float y) { m_posX = x; m_posY = y; }
		void SetActive(bool active) { m_active = active; }
		void SetForceInput(bool use, short xInput, bool dashInput) { m_forceInput = use; m_forceInputX = xInput; m_forceInputDash = dashInput; }
		void SetVelocity(float x, float y) { m_velX = x, m_velY = y; }

		void UpdateMovement();
		void MoveViewport();
		void ClampPosToViewport();

		void CheckGround();
		bool IsGround() { return m_isGrounded; }

		void CheckCollision();
		void OnlyCheckStaticCollision();

		float GetPosX() const { return m_posX; }
		float GetPosY() const { return m_posY; }
		float GetVelX() const { return m_velX; }
		float GetVelY() const { return m_velY; }
		float GetAdditionalVelX() const { return m_additianalVelX; }
		bool GetJumpTrigger() const { return m_jumpTrigger; }
		float GetJumpTimer() const { return m_jumpTimer; }
		const SPRITE* GetSprite() const { return &m_spr; }
		void ForceChangePlayerShape(int idx);
	private:
		static constexpr const WCHAR* m_playerShape1[PLAYER_SPR_SIZE_Y] = {
		L"\0/\\/\\",
		L"\0(-.-)",
		L"~U   )\0",
		};
		static constexpr const WCHAR* m_playerShape2[PLAYER_SPR_SIZE_Y] = {
		L"\0/\\/\\",
		L"\0(-.-)",
		L"-(U  )",
		};
		static constexpr const WCHAR* m_playerShape3[PLAYER_SPR_SIZE_Y] = {
		L"\0/\\/\\",
		L"\0(-.-O",
		L"^(   )",
		};
		static constexpr const WCHAR* m_playerGrabShape[PLAYER_SPR_SIZE_Y] = {
		L"\0/\\/\\",
		L"\0(o.o)",
		L"~(   >",
		};
		static constexpr const WCHAR* m_playerDieShape[PLAYER_SPR_SIZE_Y] = {
		L"\0/\\/\\",
		L"\0(x.x)",
		L" (   )\0",
		};

		bool m_active;

		PlayScene* m_scene = nullptr;

		bool m_forceInput;
		float m_forceInputX;
		float m_forceInputDash;

		SPRITE m_spr;
		float m_posX;
		float m_posY;
		float m_step;
		short m_currentSprNum;
		float m_isGrounded;
		float m_jumpTimer;
		bool m_jumpTrigger;
		float m_velX;
		float m_velY;
		float m_additianalVelX;
	};
}
