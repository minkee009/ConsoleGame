#pragma once
#include "IScene.hpp"



class MenuScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	SceneState& GetSceneState() override { return m_sceneState; };

private:
	int m_menuPointer;
	SceneState m_sceneState = Menu;


};