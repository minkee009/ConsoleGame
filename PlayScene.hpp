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
#define PLAYER_MAXFALLSPEED 40

#define PLAYER_AIR_ACCEL 20
#define PLAYER_GRAVITY 80
#define PLAYER_JUMPPOWER 30

#define MAP01_SIZE_X 90
#define MAP01_SIZE_Y 8

#define MAP01_SPR_SIZE_X 6
#define MAP01_SPR_SIZE_Y 3

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
	static const WORD m_commonShapeColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

	static constexpr const char* m_map01[MAP01_SIZE_Y] = {
		".........ggd..............................................................................",
		"..........................................................................................",
		"..........................................................................................",
		"........gg.g..............................................................................",
		"g...............gg........................................................................",
		".g.............ggd.....gg.................................................................",
		"...g.........Egdgg....ggdg....gdgg.....gg.................................................",
		"gdggggdgggdggggddggggggdgg..gggggggdgggggg.gg.dg.ggd.ggdg.ggddggggggdgg..gggggggdggggggggg"
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

	WCHAR** m_renderedMap; //[MAP01_SIZE_Y * MAP01_SPR_SIZE_Y][MAP01_SIZE_X * MAP01_SPR_SIZE_X];

	float m_playerPosX;
	float m_playerPosY;

	float m_playerStep;
	short m_currentSprNum;

	float m_isGrounded;

	float m_player_velX;
	float m_player_velY;

	SPRITE m_playerSpr;

	SPRITE m_map01_groundSpr;
	SPRITE m_map01_dgroundSpr;

	SceneState m_sceneState = Play;
};