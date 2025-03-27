#pragma once
#include "IScene.hpp"

#define MENUSENE_MENUPOINTER_MAXROW 2

class MenuScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void CreateDrawCall() override;
	SceneState& GetSceneState() override { return m_sceneState; };

private:
	int m_menuPointer;
	SceneState m_sceneState = Menu;


};