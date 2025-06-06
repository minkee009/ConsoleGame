﻿#pragma once
#include "IScene.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "Enemy.hpp"
#include "Item.hpp"
#include "PointPrinter.hpp"
#include <vector>
#include <queue>
#include <utility>
#include "ObjectManager.hpp"

#define MAP01_SIZE_X 150
#define MAP01_SIZE_Y 10

#define TILE_SPR_SIZE_X 6
#define TILE_SPR_SIZE_Y 3

#define MSG_BUFFER_SIZE 1024

#define PLAY_GOALINCOUNT_00_TIMER 1.0f
#define PLAY_GOALINCOUNT_02_TIMER 1.2f
#define PLAY_GOALINCOUNT_04_TIMER 2.0f

#define PLAY_DEADINCOUNT_00_TIMER 0.7f
#define PLAY_DEADINCOUNT_01_TIMER 2.5f

#define PRINTPOINT_VELY 15.0f

#define IS_PLAYER 1
#define IS_TILE 2
#define IS_ITEM 4
#define IS_ENEMY 8


namespace MyGame
{
	enum GameState
	{
		PrintLife,
		Playing,
		Goal,
		PlayerDead,
	};

	class Tile;
	class Player;
	class PlayScene : public IScene
	{
	public:
		GameState gameState = PrintLife;

		void Initialize() override;
		void Update() override;
		void Render() override;
		SceneState& GetSceneState() override { return m_sceneState; };

		PlayScene();
		~PlayScene();

		bool CheckTileCollision(char c)
		{
			return c == 'g' || c == 'd' || c == 'y' || c == 'u' || c == 'j' || c == 'k' || c == 's';
		}

		void SetChildMode(bool mode) { m_childMode = mode; }

		void AddTile(Tile* tile) { m_objManager->addedTiles.push(tile); };
		void AddEnemy(Enemy* enemy) { m_objManager->addedEnemys.push(enemy); };
		void AddItem(Item* item) { m_objManager->addedItems.push(item); }
		void AddScore(float score) { m_score += score; }
		void PrintPoint(const wchar_t* point, float posX, float posY) { m_pointPrinter->CreatePoint(point, 0.5f,posX,posY, 0.0f, -PRINTPOINT_VELY); }
		const std::vector<std::pair<Tile*, bool>>* GetTiles() { return &m_objManager->tiles; }
		const std::vector<std::pair<Enemy*, bool>>* GetEnemys() { return &m_objManager->enemys; }
		const std::vector<std::pair<Item*, bool>>* GetItems() { return &m_objManager->items; }

		static constexpr const char* const* GetTilemap() { return m_map01; }
		Player* GetPlayer() { return m_player; }

	private:

		float CorrectPosY(float spriteSizeY)
		{
			auto correctY = abs(spriteSizeY - TILE_SPR_SIZE_Y);
			return spriteSizeY > TILE_SPR_SIZE_Y ? -correctY : correctY;
		}

		static const WORD m_commonShapeColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		
		/// <summary>
		///                               { hide   ~      } {       @             } {      #        } {   $          }
		/// { B : block } { 0 : coin mb } { 1 : goomba mb } { 2 : dummy goomba mb } { 3 : jumper mb } { 4 : ninja mb }
		/// { e : goomba } { F : jumper } { N : ninja } { D : dummy goomba }
		/// { g : ground } { d : dirt } { y : pipe0 } { u : pipe1 } { j : pipe2 } { k : pipe3 } { s : solid block } { i : goal flag }
		/// </summary>


		static constexpr const char* m_map01[MAP01_SIZE_Y] = {
			".................cv.........cv................cv......................................................................................................",
			".........cv.............................cv..................cv.......cv...............................................................................",
			"...................................0.....................................................~............................................................",
			"...........................................................................................CC.........................................................",
			".............................~....CCC...........................N..............B3BB.......C..C.....................................s..................",
			".................................B5B0B.........$.....e........BBBBB............................e..................................ss..................",
			"....................................................yu................e.................s.....yu~~~~.............................sss..................",
			".......................CCC.yu.......................jk...............ss................ss.....jk....ss..........................ssss..................",
			"...............F...yu....e.jk.....e............F....jk...ss...D.....ssss......yu......sss.....jkCCCCss..D.........F...N...N....sssss.....i............",
			"gdggggdgggdggggddggggggdgggggggggggdgggggggdggdgdggddggdgdggddggggggdggddgggggggdgggggggg....ggggg..gggggggggggggggggggggggggggggggggggggggggggggggggg",
		};

		//static constexpr const char* m_map01[MAP01_SIZE_Y] = {
		//	".................cv.........cv................cv..........................................",
		//	".........cv.........................cv......................cv.......cv...................",
		//	"..........................................................................................",
		//	"..........................................................................................",
		//	"........~.....................................N...........................................",
		//	"...................................e........BBBBB.........................................",
		//	"..................................yu................e.....................................",
		//	"...........................yu.....jk...............ss.....................................",
		//	"...............F...yu....e.jk.....jk...ss...D.....ssss...........F...N...N...i............",
		//	"gdggggdgggdggggddggggggdgggggggggggdgggggggdggdgdggddggdgdggddggggggdggddgggggggdggggggggg",
		//};


		static constexpr const WCHAR* m_map01_gShape[TILE_SPR_SIZE_Y] = {
			L"▓▓██▓▓",
			L"██▒▒██",
			L"█▒▒▓▓█",
		};

		static constexpr const WCHAR* m_map01_dShape[TILE_SPR_SIZE_Y] = {
			L"▓▓█▒▒▓",
			L"█▓▓▓██",
			L"▒███▓▓",
		};

		static constexpr const WCHAR* m_map01_pipe0Shape[TILE_SPR_SIZE_Y] = {
			L"▓▓████",
			L" ▒▒▓██",
			L" ▒▒▓██",
		};
		static constexpr const WCHAR* m_map01_pipe1Shape[TILE_SPR_SIZE_Y] = {
			L"████▓▓",
			L"██▓▒▒ ",
			L"██▓▒▒ ",
		};
		static constexpr const WCHAR* m_map01_pipe2Shape[TILE_SPR_SIZE_Y] = {
			L" ▒▒▓██",
			L" ▒▒▓██",
			L" ▒▒▓██",
		};
		static constexpr const WCHAR* m_map01_pipe3Shape[TILE_SPR_SIZE_Y] = {
			L"██▓▒▒ ",
			L"██▓▒▒ ",
			L"██▓▒▒ ",
		};

		static constexpr const WCHAR* m_map01_solidBlock[TILE_SPR_SIZE_Y] = {
			L"█████ ",
			L"██▓▓  ",
			L"█     ",
		};

		static constexpr const WCHAR* m_map01_cloud0[TILE_SPR_SIZE_Y] = {
			L"     .",
			L"  .-( ",
			L" (___.",
		};

		static constexpr const WCHAR* m_map01_cloud1[TILE_SPR_SIZE_Y] = {
			L"--.   ",
			L"   ). ",
			L"__)__)",
		};

		//게임 오브젝트 -> { 오브젝트, 인스턴스 여부 }
		ObjectManager* m_objManager;

		WCHAR** m_renderedMap; //[MAP01_SIZE_Y * MAP01_SPR_SIZE_Y][MAP01_SIZE_X * MAP01_SPR_SIZE_X];
		PointPrinter* m_pointPrinter = nullptr;
		Player* m_player = nullptr;

		SceneState m_sceneState = Play;

		bool m_childMode = false;
		short m_goalIncount;
		float m_goalTimer;

		short m_deadIncount;
		float m_deadTimer;

		int m_life = 3;
		float m_score = 0;
		float m_timer;
		bool m_pause = false;
		short m_menuIdx = 0;

		wchar_t* m_msgBuffer;
	};
}

