#pragma once
#include "IScene.hpp"

class PlayScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	SceneState& GetSceneState() override { return m_sceneState; };

private:
	COORD m_playerPos;
	SPRITE m_playerSpr;
	SceneState m_sceneState = Play;
};