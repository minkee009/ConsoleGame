#include "input.hpp"

// 정적 멤버 변수 정의
SHORT MyGame::Input::_oldKeyState[KEY_SIZE] = { 0 };
SHORT MyGame::Input::_curKeyState[KEY_SIZE] = { 0 };

void MyGame::Input::ScanKey()
{
	memcpy(_oldKeyState, _curKeyState, sizeof(SHORT) * KEY_SIZE);
	for (int i = 0; i < KEY_SIZE; i++)
	{
		_curKeyState[i] = GetAsyncKeyState(i);
	}
}


bool MyGame::Input::GetKey(int32_t keyCode)
{
	return (_curKeyState[keyCode] & KEY_PRESSED);
}

bool MyGame::Input::GetKeyDown(int32_t keyCode)
{
	return !(_oldKeyState[keyCode] & KEY_PRESSED) && (_curKeyState[keyCode] & KEY_PRESSED);
}


bool MyGame::Input::GetKeyUp(int32_t keyCode)
{
	return (_oldKeyState[keyCode] & KEY_PRESSED) && !(_curKeyState[keyCode] & KEY_PRESSED);
}