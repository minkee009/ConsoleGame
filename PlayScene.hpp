#pragma once
#include "IScene.hpp"

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

#define MAP01_SIZE_X 90
#define MAP01_SIZE_Y 8

#define MAP01_SPR_SIZE_X 6
#define MAP01_SPR_SIZE_Y 3


#define GOAL_FLAG_SPR_SIZE_X 2
#define GOAL_FLAG_SPR_SIZE_Y 16

class PlayScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	SceneState& GetSceneState() override { return m_sceneState; };

	PlayScene();
	~PlayScene();

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

	float m_playerPosX;
	float m_playerPosY;

	float m_playerStep;
	short m_currentSprNum;

	float m_isGrounded;
	float m_jumpTimer;
	bool m_jumpTrigger;

	float m_player_velX;
	float m_player_velY;

	SPRITE m_playerSpr;

	SPRITE m_map01_groundSpr;
	SPRITE m_map01_dgroundSpr;

	SPRITE m_map01_pipe0Spr;
	SPRITE m_map01_pipe1Spr;
	SPRITE m_map01_pipe2Spr;
	SPRITE m_map01_pipe3Spr;
	SPRITE m_map01_solidBlockSpr;

	SPRITE m_goalFlagSpr;

	SceneState m_sceneState = Play;


	bool CheckTileCollision(char c)
	{
		return c == 'g' || c == 'd' || c == 'y' || c == 'u' || c == 'j' || c == 'k' || c == 's';
	}
};



