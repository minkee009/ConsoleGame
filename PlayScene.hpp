#pragma once
#include "IScene.hpp"
#include "Player.hpp"

#define MAP01_SIZE_X 90
#define MAP01_SIZE_Y 8

#define MAP01_SPR_SIZE_X 6
#define MAP01_SPR_SIZE_Y 3


#define GOAL_FLAG_SPR_SIZE_X 2
#define GOAL_FLAG_SPR_SIZE_Y 16



namespace MyGame
{
	class Player;
	class PlayScene : public IScene
	{
	public:
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

		static constexpr const char* const* GetTilemap() { return m_map01; }

	private:
		static const WORD m_commonShapeColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

		static constexpr const char* m_map01[MAP01_SIZE_Y] = {
			"..........................................................................................",
			"..........................................................................................",
			"..........................................................................................",
			"..................................yu......................................................",
			"...........................yu.....jk......................................................",
			"...................yu......jk.....jk...............ss.....................................",
			"...................jk......jk.....jk...gg.........ssss....................................",
			"gdggggdgggdggggddggggggdgggggggggggdgggggggdggdgdggddggdgdggddggggggdggddgggggggdggggggggg",
		};

		static constexpr const WCHAR* m_map01_gShape[MAP01_SPR_SIZE_Y] = {
			L"▓▓██▓▓",
			L"██▒▒██",
			L"█▒▒▓▓█",
		};

		static constexpr const WCHAR* m_map01_dShape[MAP01_SPR_SIZE_Y] = {
			L"▓▓█▒▒▓",
			L"█▓▓▓██",
			L"▒███▓▓",
		};

		static constexpr const WCHAR* m_map01_pipe0Shape[MAP01_SPR_SIZE_Y] = {
			L"▓▓████",
			L" ▒▒▓██",
			L" ▒▒▓██",
		};
		static constexpr const WCHAR* m_map01_pipe1Shape[MAP01_SPR_SIZE_Y] = {
			L"████▓▓",
			L"██▓▒  ",
			L"██▓▒  ",
		};
		static constexpr const WCHAR* m_map01_pipe2Shape[MAP01_SPR_SIZE_Y] = {
			L" ▒▒▓██",
			L" ▒▒▓██",
			L" ▒▒▓██",
		};
		static constexpr const WCHAR* m_map01_pipe3Shape[MAP01_SPR_SIZE_Y] = {
			L"██▓▒  ",
			L"██▓▒  ",
			L"██▓▒  ",
		};

		static constexpr const WCHAR* m_map01_solidBlock[MAP01_SPR_SIZE_Y] = {
			L"█████ ",
			L"██▓▓    ",
			L"█     ",
		};

		static constexpr const WCHAR* m_goalShape[GOAL_FLAG_SPR_SIZE_Y] = {
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		L"█▓",
		};


		WCHAR** m_renderedMap; //[MAP01_SIZE_Y * MAP01_SPR_SIZE_Y][MAP01_SIZE_X * MAP01_SPR_SIZE_X];

		COORD m_playerSpawnPos;

		Player* m_player = nullptr;

		SPRITE m_map01_groundSpr;
		SPRITE m_map01_dgroundSpr;

		SPRITE m_map01_pipe0Spr;
		SPRITE m_map01_pipe1Spr;
		SPRITE m_map01_pipe2Spr;
		SPRITE m_map01_pipe3Spr;
		SPRITE m_map01_solidBlockSpr;

		SPRITE m_goalFlagSpr;

		SceneState m_sceneState = Play;
	};
}

