#pragma once
#include "Enemy.hpp"
#include "Tile.hpp"

namespace MyGame
{
	class Tile;
	class Enemy;
	class ObjectManager
	{
	public:
		std::vector<std::pair<Tile*, bool>> tiles;
		std::vector<std::pair<Enemy*, bool>> enemys;

		std::queue<Tile*> addedTiles;
		std::queue<Enemy*> addedEnemys;
	};
}