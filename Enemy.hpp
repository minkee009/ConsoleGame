#pragma once
#include "PlayScene.hpp"

namespace MyGame
{
	class PlayScene;
	class Enemy
	{
	public:
		void SetSpawnPos(float x, float y) { m_spawnPosX = x; m_spawnPosY = y; }
		void SetPosition(float x, float y) { m_posX = x; m_posY = y; }
		void SetActive(bool active) { m_active = active; }
		void SetForceIgnoreCollision(bool force) { m_forceIgnoreCollision = force; }
		bool IsAlive() { return m_isAlive; }
		bool IsActtacked() { return m_attacked; }
		bool GetActive() { return m_active && m_isAlive; }
		virtual void Initialize() {}
		virtual void Update() {}
		virtual void CheckCollision() {}
		virtual void CallInteract(int collisionFlag) {}
		float GetPosX() { return m_posX; }
		float GetPosY() { return m_posY; }
		float GetSpawnPosX() { return m_spawnPosX; }
		float GetSpawnPosY() { return m_spawnPosY; }
		bool IsDontPress() { return m_dontPress; }
		Bbox GetCheckBBox() const {
			return {
			m_posX - m_spr.Size.X + (m_spr.Size.X << 1),
			m_posY - m_spr.Size.Y + (m_spr.Size.X << 1),
			m_posX - m_spr.Size.X,
			m_posY - m_spr.Size.Y };
		}
		Bbox GetBbox() const { return { m_posX + m_spr.Size.X,m_posY + m_spr.Size.Y,m_posX,m_posY }; }
		const SPRITE* GetSprite() { return &m_spr; }
		bool GetForceIgnoreCollision() { return m_forceIgnoreCollision; }

		virtual ~Enemy() {}
		virtual Enemy* Clone() const = 0;

	protected:
		float m_spawnPosX;
		float m_spawnPosY;
		float m_posX;
		float m_posY;
		float m_velX;
		float m_velY;
		bool m_attacked;
		bool m_dontPress;
		bool m_active;
		bool m_isAlive;
		bool m_forceIgnoreCollision = false;
		PlayScene* m_scene;
		SPRITE m_spr;
	};
}