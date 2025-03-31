#pragma once
#include "Tile.hpp"
#include "Enemy.hpp"
#include "Item.hpp"

#define BLOCK_SPR_SIZE_X 6
#define BLOCK_SPR_SIZE_Y 3

namespace MyGame
{
	class MysteryBlock : public Tile
	{
	public:
		MysteryBlock(PlayScene* scene);

		void SetHide() { m_hide = true;  m_isOnlyDownHit = true; m_initDownHit = true; }
		void Initialize() override;
		void Update() override;
		void CallInteract(int collisionFlag) override;
		void SetInItem(Item* item) { m_item = item; }
		void SetInEnemy(Enemy* enemy) { m_enemy = enemy; }
	private:
		static constexpr const WCHAR* m_blockShape[BLOCK_SPR_SIZE_Y] = {
			L"⡎⢉⡽⢿⡍⢱",
			L"⡇⠈⢡⡾⠃⢸",
			L"⢇⣀⣀⣗⣀⡸",
		};
		static constexpr const WCHAR* m_blockHitShape[BLOCK_SPR_SIZE_Y] = {
			L"⡎⠉⠉⠉⠉⢱",
			L"⡇⠀⠀⠀⠀⢸",
			L"⢇⣀⣀⣀⣀⡸",
		};
		static constexpr const WCHAR* m_hideshape[BLOCK_SPR_SIZE_Y] = {
			L"      ",
			L"      ",
			L"      ",
		};

		bool m_isHit;
		float m_timer;
		bool m_hide;
		bool m_created;
		bool m_initDownHit;
		Enemy* m_enemy = nullptr;
		Enemy* m_prevEnemy = nullptr;
		Item* m_item = nullptr;
		Item* m_prevItem = nullptr;
	};
}