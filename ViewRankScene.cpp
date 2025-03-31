#include "ViewRankScene.hpp"
#include "engine.hpp"

void MyGame::ViewRankScene::Initialize()
{
	m_menuPointer = 0;
    // 파일 이름 설정
    std::string filename = "saved.rank";

    // 파일에서 랭킹 불러오기
    m_rankings = LoadRankingFromFile(filename);
    m_cursor_visible = true;
    m_timer = 0;
}

void MyGame::ViewRankScene::Update()
{
    m_timer += GET_DELTATIME();

    if (m_timer > 0.5f)
    {
        m_timer = 0;
        m_cursor_visible = !m_cursor_visible;
    }

    if(GET_KEY_DOWN(VK_RETURN) || GET_KEY_DOWN(VK_SPACE))
        CHANGE_SCENE(Menu);
}

void MyGame::ViewRankScene::Render()
{
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 7), 6 }, L"~ 명예의 전당 ~              ");

    if (m_rankings.empty())
    {
        RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 28), 10 }, L"아직 도전하신 분이 없습니다. 지금 당장 게임을 하러가시죠!");

        RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 8),(SHORT)(34) }, L"메뉴로 돌아가기");
        RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 10),(SHORT)(34) }, m_cursor_visible ? L">" : L" ");
    }
    else 
    {
        int idx = 0;
        DWORD color = FOREGROUND_INTENSITY;
        for (idx = 0; idx < 10 && idx < m_rankings.size(); idx++)
        {
            if (idx == 0)
                color = FOREGROUND_RED | FOREGROUND_GREEN ;
            if (idx == 1)
                color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            if (idx == 2)
                color = FOREGROUND_RED | FOREGROUND_INTENSITY;
            if (idx == 3)
                color = FOREGROUND_INTENSITY;

            swprintf_s(m_output_buffer, 100, L"%d위 -  ", idx + 1);
            RENDER_STR_COLOR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 30),(SHORT)(10 + idx * 2) }, m_output_buffer, color);
            // UTF-8에서 UTF-16으로 변환할 공간의 크기 계산
            int len = MultiByteToWideChar(CP_UTF8, 0, m_rankings[idx].name.c_str(), -1, NULL, 0);
            std::wstring wide_str(len, L'\0'); // 변환된 결과를 담을 wstring 생성
            // 실제 변환
            MultiByteToWideChar(CP_UTF8, 0, m_rankings[idx].name.c_str(), -1, &wide_str[0], len);
            swprintf_s(m_output_buffer, 100, L" %s     ", wide_str.c_str());
            RENDER_STR_COLOR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 24),(SHORT)(10 + idx * 2) }, m_output_buffer, color);
            swprintf_s(m_output_buffer, 100, L"%d", m_rankings[idx].score);
            RENDER_STR_COLOR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 8),(SHORT)(10 + idx * 2) }, L"··········          ", color);
            RENDER_STR_COLOR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) + 15),(SHORT)(10 + idx * 2) }, m_output_buffer, color);
        }

        if (m_rankings.size() > 10)
        {
            swprintf_s(m_output_buffer, 100, L"그 외의 %d분이 명예의 전당에 오르기 위해 사투를 벌였지만 안탑깝게도 등재되지 못했습니다...                                           ", m_rankings.size() - idx);
            RENDER_STR({ 15,(SHORT)(31) }, m_output_buffer);
        }

        RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 8),(SHORT)(34) }, L"메뉴로 돌아가기");
        RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 10),(SHORT)(34) }, m_cursor_visible ? L">" : L" ");
    }
}



