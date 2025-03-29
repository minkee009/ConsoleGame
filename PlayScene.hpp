#pragma once
#include "IScene.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include <vector>
#include <utility>

#define MAP01_SIZE_X 90
#define MAP01_SIZE_Y 10

#define TILE_SPR_SIZE_X 6
#define TILE_SPR_SIZE_Y 3

#define MSG_BUFFER_SIZE 1024

#define PLAY_GOALINCOUNT_00_TIMER 1.0f
#define PLAY_GOALINCOUNT_02_TIMER 1.2f
#define PLAY_GOALINCOUNT_04_TIMER 2.0f

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

		void AddTile(Tile* tile) { m_tiles.push_back({ tile,true }); };

		const std::vector<std::pair<Tile*, bool>>* GetTiles() { return &m_tiles; };

		static constexpr const char* const* GetTilemap() { return m_map01; }

	private:

		float CorrectPosY(float spriteSizeY)
		{
			auto correctY = abs(spriteSizeY - TILE_SPR_SIZE_Y);
			return spriteSizeY > TILE_SPR_SIZE_Y ? -correctY : correctY;
		}

		static const WORD m_commonShapeColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		
		/// <summary>
		/// { g : ground } { d : dirt } { y : pipe0 } { u : pipe1 } { j : pipe2 } { k : pipe3 } { s : solid block } { i : goal flag }
		/// </summary>
		static constexpr const char* m_map01[MAP01_SIZE_Y] = {
			".................cv.........cv................cv..........................................",
			".........cv.........................cv......................cv.......cv...................",
			"..........................................................................................",
			"..........................................................................................",
			"..........................................................................................",
			"..................................yu......................................................",
			"...........................yu.....jk......................................................",
			"...................yu......jk.....jk...............ss.....................................",
			"...................jk......jk.....jk...gg.........ssss.......................i............",
			"gdggggdgggdggggddggggggdgggggggggggdgggggggdggdgdggddggdgdggddgg...gdggddgggggggdggggggggg",
		};

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
			L"██▓▒  ",
			L"██▓▒  ",
		};
		static constexpr const WCHAR* m_map01_pipe2Shape[TILE_SPR_SIZE_Y] = {
			L" ▒▒▓██",
			L" ▒▒▓██",
			L" ▒▒▓██",
		};
		static constexpr const WCHAR* m_map01_pipe3Shape[TILE_SPR_SIZE_Y] = {
			L"██▓▒  ",
			L"██▓▒  ",
			L"██▓▒  ",
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

		//타일 오브젝트 -> { 타일, 인스턴스 여부 }
		std::vector<std::pair<Tile*, bool>> m_tiles;
		std::vector<std::pair<Tile*, bool>> m_items;
		std::vector<std::pair<Tile*, bool>> m_enemys;


		WCHAR** m_renderedMap; //[MAP01_SIZE_Y * MAP01_SPR_SIZE_Y][MAP01_SIZE_X * MAP01_SPR_SIZE_X];

		Player* m_player = nullptr;

		SceneState m_sceneState = Play;

		short m_goalIncount;
		float m_goalTimer;
		int m_life = 3;
		float m_score = 0;
		float m_timer;

		wchar_t* m_msgBuffer;
	};
}

