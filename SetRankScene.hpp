#pragma once
#include "IScene.hpp"
#include <vector>
#include <string>

#define SETRANKSCENE_MENUPOINTER_MAXROW 7
#define SETRANKSCENE_MENUPOINTER_MAXCOL 4
#define SETRANKSCENE_MAXNAME_COUNT 8

namespace MyGame
{
	struct Ranking {
		std::string name;
		int score;
	};

	class SetRankScene : public IScene
	{
	public:
		void Initialize() override;
		void Update() override;
		void Render() override;
		SceneState& GetSceneState() override { return m_sceneState; };

		void SetCurrentScore(int score) { m_currentScore = score; }
	private:
		int m_menuPointer;
		int m_currentScore;
		bool m_isHighScore;
		wchar_t m_name[SETRANKSCENE_MAXNAME_COUNT + 1];
		wchar_t m_nullchar[SETRANKSCENE_MAXNAME_COUNT + 1];
		float m_timer;
		bool m_cursor_visible;

		int m_current_char_idx;

		const wchar_t* m_alphabets[4] = {
			L"ABCDEFG",
			L"HIJKLMN",
			L"QRSTUVW",
			L"XYZ_ "
		};

		int m_cursorX;
		int m_cursorY;

		SceneState m_sceneState = SetRank;
        std::vector<MyGame::Ranking> LoadRankingFromFile(const std::string& filename);
        void SaveRankingToFile(const std::string& filename, const std::vector<MyGame::Ranking>& rankings);
	};
}
