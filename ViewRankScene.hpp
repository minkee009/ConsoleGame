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
		bool m_cursor_visible;

		std::vector<MyGame::Ranking> m_rankings;

		SceneState m_sceneState = ViewRank;
		//std::vector<MyGame::Ranking> LoadRankingFromFile(const std::string& filename);
		//void SaveRankingToFile(const std::string& filename, const std::vector<MyGame::Ranking>& rankings);
	};
}
