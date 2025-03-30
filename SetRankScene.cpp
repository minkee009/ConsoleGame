#include "SetRankScene.hpp"
#include "engine.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <locale>
#include <codecvt>

void MyGame::SetRankScene::Initialize()
{
    // 파일 이름 설정
    std::string filename = "saved.rank";

    // 파일에서 랭킹 불러오기
    m_rankings = LoadRankingFromFile(filename);

    if (m_rankings.empty() || m_rankings[0].score < m_currentScore)
        m_isHighScore = true;
    else
    {
        m_isHighScore = false;
        swprintf_s(high_score_txt, 100, L"High Score :  %8d", (int)m_rankings[0].score);
    }

    for (size_t i = 0; i < SETRANKSCENE_MAXNAME_COUNT; i++)
    {
        m_nullchar[i] = L'_';
        m_name[i] = L'_';
    }

    swprintf_s(score_txt, 100, L"Score :  %8d", (int)m_currentScore);


    m_nullchar[SETRANKSCENE_MAXNAME_COUNT] = L'\0';
    m_name[SETRANKSCENE_MAXNAME_COUNT] = L'\0';

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
        m_cursorY = m_cursorY < 0 ? 4 : m_cursorY;
        m_cursorY %= 5;
    }
    if (GET_KEY_DOWN(VK_DOWN))
    {
        m_cursorY++;
        m_cursorY %= 5;
    }
    if (GET_KEY_DOWN(VK_LEFT))
    {
        m_cursorX--;
        m_cursorX = m_cursorX < 0 ? 6 : m_cursorX;
        m_cursorX %= 7;
    }
    if (GET_KEY_DOWN(VK_RIGHT))
    {
        m_cursorX++;
        m_cursorX %= 7;
    }
    if (GET_KEY_DOWN(VK_RETURN))
    {
        if (m_cursorX == 5 && m_cursorY == 4)
        {
            m_current_char_idx--;
            m_current_char_idx = m_current_char_idx < 0 ? SETRANKSCENE_MAXNAME_COUNT - 1 : m_current_char_idx;
            m_current_char_idx %= SETRANKSCENE_MAXNAME_COUNT;
            return;
        }
        if (m_cursorX == 6 && m_cursorY == 4)
        {
            std::string filname = "saved.rank";
            std::wstring wstr(m_name);
            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            std::string str = conv.to_bytes(wstr);
            m_rankings.push_back({ str,(int)m_currentScore });

            CHANGE_SCENE(Menu);
            SaveRankingToFile(filname, m_rankings);
            return;
        }
        m_name[m_current_char_idx] = m_alphabets[m_cursorY][m_cursorX];
        ++m_current_char_idx %= SETRANKSCENE_MAXNAME_COUNT;
    }
}

void MyGame::SetRankScene::Render()
{
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 7), 8 }, m_isHighScore ? L"최고기록 갱신!             " : L" 당신의 기록           ");
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 10), 10 }, score_txt);
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 15), 12 }, m_isHighScore ? L"" : high_score_txt);
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 6), 14 }, m_nullchar);
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 6), 14 }, m_name);
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 6 + m_current_char_idx), 15 }, L"^");

    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 20), 19 }, L"A     B     C     D     E     F     G");
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 20), 22 }, L"H     I     J     K     L     M     N");
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 20), 25 }, L"O     P     Q     R     S     T     U");
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 20), 28 }, L"V     W     X     Y     Z     _     Spc");
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 20), 31 }, L"인    재    원    7    기    Bck   End");
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 22 + (m_cursorX * 6)), (SHORT)(19 + m_cursorY * 3) }, m_cursor_visible ? L">" : L" ");

}
