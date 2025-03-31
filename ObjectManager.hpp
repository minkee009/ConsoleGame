#pragma once
#include "Enemy.hpp"
#include "Tile.hpp"
#include "Item.hpp"

namespace MyGame
{
	class Tile;
	class Enemy;
	class Item;
	class ObjectManager
	{
	public:
		std::vector<std::pair<Tile*, bool>> tiles;
		std::vector<std::pair<Enemy*, bool>> enemys;
		std::vector<std::pair<Item*, bool>> items;

		std::queue<Tile*> addedTiles;
		std::queue<Enemy*> addedEnemys;
		std::queue<Item*> addedItems;
	};
}