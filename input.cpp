#include "input.hpp"

// 정적 멤버 변수 정의
SHORT MyGame::Input::m_oldKeyState[KEY_SIZE] = { 0 };
SHORT MyGame::Input::m_curKeyState[KEY_SIZE] = { 0 };

void MyGame::Input::ScanKey()
{
	memcpy(m_oldKeyState, m_curKeyState, sizeof(SHORT) * KEY_SIZE);
	for (int i = 0; i < KEY_SIZE; i++)
	{
		m_curKeyState[i] = GetAsyncKeyState(i);
	}
}


bool MyGame::Input::GetKey(int32_t keyCode)
{
	return (m_curKeyState[keyCode] & KEY_PRESSED);
}

bool MyGame::Input::GetKeyDown(int32_t keyCode)
{
	return !(m_oldKeyState[keyCode] & KEY_PRESSED) && (m_curKeyState[keyCode] & KEY_PRESSED);
}


bool MyGame::Input::GetKeyUp(int32_t keyCode)
{
	return (m_oldKeyState[keyCode] & KEY_PRESSED) && !(m_curKeyState[keyCode] & KEY_PRESSED);
}