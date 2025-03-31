#include "ViewRankScene.hpp"
#include "engine.hpp"

void MyGame::ViewRankScene::Initialize()
{
	m_menuPointer = 0;
    // ���� �̸� ����
    std::string filename = "saved.rank";

    // ���Ͽ��� ��ŷ �ҷ�����
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
    RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 7), 6 }, L"~ ���� ���� ~              ");

    if (m_rankings.empty())
    {
        RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 28), 10 }, L"���� �����Ͻ� ���� �����ϴ�. ���� ���� ������ �Ϸ�������!");

        RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 8),(SHORT)(34) }, L"�޴��� ���ư���");
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

            swprintf_s(m_output_buffer, 100, L"%d�� -  ", idx + 1);
            RENDER_STR_COLOR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 30),(SHORT)(10 + idx * 2) }, m_output_buffer, color);
            // UTF-8���� UTF-16���� ��ȯ�� ������ ũ�� ���
            int len = MultiByteToWideChar(CP_UTF8, 0, m_rankings[idx].name.c_str(), -1, NULL, 0);
            std::wstring wide_str(len, L'\0'); // ��ȯ�� ����� ���� wstring ����
            // ���� ��ȯ
            MultiByteToWideChar(CP_UTF8, 0, m_rankings[idx].name.c_str(), -1, &wide_str[0], len);
            swprintf_s(m_output_buffer, 100, L" %s     ", wide_str.c_str());
            RENDER_STR_COLOR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 24),(SHORT)(10 + idx * 2) }, m_output_buffer, color);
            swprintf_s(m_output_buffer, 100, L"%d", m_rankings[idx].score);
            RENDER_STR_COLOR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 8),(SHORT)(10 + idx * 2) }, L"��������������������          ", color);
            RENDER_STR_COLOR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) + 15),(SHORT)(10 + idx * 2) }, m_output_buffer, color);
        }

        if (m_rankings.size() > 10)
        {
            swprintf_s(m_output_buffer, 100, L"�� ���� %d���� ���� ���翡 ������ ���� ������ �������� ��ž���Ե� ������� ���߽��ϴ�...                                           ", m_rankings.size() - idx);
            RENDER_STR({ 15,(SHORT)(31) }, m_output_buffer);
        }

        RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 8),(SHORT)(34) }, L"�޴��� ���ư���");
        RENDER_STR({ (SHORT)((GET_SCREEN_WIDTH() >> 1) - 10),(SHORT)(34) }, m_cursor_visible ? L">" : L" ");
    }
}



