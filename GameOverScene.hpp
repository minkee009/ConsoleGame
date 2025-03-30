#pragma once
#include "IScene.hpp"

namespace MyGame
{
	class GameOverScene : public IScene
	{
	public:
		void Initialize() override;
		void Update() override;
		void Render() override;
		SceneState& GetSceneState() override { return m_sceneState; };
	private:
		SceneState m_sceneState = GameOver;
		float m_timer;
	};
}
