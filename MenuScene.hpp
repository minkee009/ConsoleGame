#pragma once
#include "IScene.hpp"

#define MENUSCENE_MENUPOINTER_MAXROW 2

namespace MyGame
{
	class MenuScene : public IScene
	{
	public:
		void Initialize() override;
		void Update() override;
		void Render() override;
		SceneState& GetSceneState() override { return m_sceneState; };
	private:
		int m_menuPointer;
		SceneState m_sceneState = Menu;
	};
}
