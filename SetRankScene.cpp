#include "SetRankScene.hpp"
#include "engine.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>



void MyGame::SetRankScene::Initialize()
{
    //// 파일 이름 설정
    //std::string filename = "saved.rank";

    //// 파일에서 랭킹 불러오기
    //std::vector<Ranking> rankings = LoadRankingFromFile(filename);

    //if (rankings.empty() || rankings[0].score < m_currentScore)
    //    m_isHighScore = true;
    //else
        m_isHighScore = false;

    for (size_t i = 0; i < SETRANKSCENE_MAXNAME_COUNT; i++)
    {
        m_nullchar[i] = L'_';
        m_name[i] = L'\0';
    }
    m_nullchar[SETRANKSCENE_MAXNAME_COUNT] = L'\0';

    m_timer = 0;
    m_current_char_idx = 0;
    m_cursorX = 0;
    m_cursorY = 0;
}

void MyGame::SetRankScene::Update()
{
    m_timer += GET_DELTATIME();

    if (m_timer > 0.25f)
    {
        m_timer = 0;
        m_cursor_visible = !m_cursor_visible;
    }

    if (GET_KEY_DOWN(VK_UP))
    {
        m_cursorY--;
        m_cursorY = m_cursorY < 0 ? 3 : m_cursorY;
        m_cursorY %= 4;
    }
    if (GET_KEY_DOWN(VK_DOWN))
    {
        m_cursorY++;
        m_cursorY %= 4;
    }
    if (GET_KEY_DOWN(VK_LEFT))
    {
        m_cursorX--;
        m_cursorX = m_cursorX < 0 ? 6 : m_cursorX;
        m_cursorY %= 7;
    }
    if (GET_KEY_DOWN(VK_RIGHT))
    {
        m_cursorX++;
        m_cursorX %= 7;
    }
    if (GET_KEY_DOWN(VK_RETURN))
    {
        if (m_cursorX == 5 && m_cursorY == 3)
        {
            m_name[m_current_char_idx] = L'\0';
            m_current_char_idx--;
            m_current_char_idx = m_current_char_idx < 0 ? SETRANKSCENE_MAXNAME_COUNT - 1 : m_current_char_idx;
            m_current_char_idx %= SETRANKSCENE_MAXNAME_COUNT;
        }
        if (m_cursorX == 6 && m_cursorY == 3)
        {
            CHANGE_SCENE(Menu);
        }
    }
}

void MyGame::SetRankScene::Render()
{
    RENDER_STR({(SHORT)((GET_SCREEN_WIDTH() >> 1) - 7), 8},m_isHighScore ? L"최고기록 갱신!" : L" 당신의 기록");
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 6), 11 }, m_nullchar);
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 6), 11 }, m_name);
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 6 + m_current_char_idx), 12 }, L"^");

    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 20), 18 }, L"A     B     C     D     E     F     G");
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 20), 21 }, L"H     I     J     K     L     M     N");
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 20), 24 }, L"P     Q     R     S     T     U     V");
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 20), 27 }, L"X     Y     Z     _     Spc   Bck   End");
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 22 +(m_cursorX * 6)), (SHORT)(18 + m_cursorY * 3)}, m_cursor_visible ? L">" : L" ");

}

// 랭킹 파일에서 불러오는 함수
std::vector<MyGame::Ranking> MyGame::SetRankScene::LoadRankingFromFile(const std::string& filename) {
    std::vector<Ranking> rankings;
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
        return rankings; // 빈 벡터 반환
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string name;
        int score;
        if (std::getline(iss, name, ',') && (iss >> score)) {
            rankings.push_back({ name, score });
        }
    }

    inFile.close();

    // 점수 기준으로 정렬
    std::sort(rankings.begin(), rankings.end(), [](const Ranking& a, const Ranking& b) {
        return b.score < a.score; // 내림차순 정렬
        });

    return rankings;
}

void MyGame::SetRankScene::SaveRankingToFile(const std::string& filename, const std::vector<MyGame::Ranking>& rankings)
{
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
        return;
    }

    for (const auto& rank : rankings) {
        outFile << rank.name << "," << rank.score << "\n";
    }

    outFile.close();
}
