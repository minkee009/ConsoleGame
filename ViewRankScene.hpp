#pragma once
#pragma once
#include "IScene.hpp"
#include "ranking_save_loader.hpp"
#include <vector>
#include <string>

namespace MyGame
{
	class ViewRankScene : public IScene
	{
	public:
		void Initialize() override;
		void Update() override;
		void Render() override;
		SceneState& GetSceneState() override { return m_sceneState; };
	private:
		int m_menuPointer;
		wchar_t m_output_buffer[100];
		std::vector<MyGame::Ranking> m_rankings;
		SceneState m_sceneState = ViewRank;
		bool m_cursor_visible;
		float m_timer;
	};
}
