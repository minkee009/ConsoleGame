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
		bool IsAlive() { return m_isAlive; }
		bool IsActtacked() { return m_attacked; }
		bool GetActive() { return m_active; }
		virtual void Initialize() {}
		virtual void Update() {}
		virtual void CheckCollision() {}
		virtual void CallInteract(int collisionFlag) {}
		float GetPosX() { return m_posX; }
		float GetPosY() { return m_posY; }
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
		PlayScene* m_scene;
		SPRITE m_spr;
	};
}