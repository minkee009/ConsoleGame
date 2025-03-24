#pragma once
#include <iostream>
#include <Windows.h>

#define KEY_SIZE 256
#define KEY_PRESSED 0x8000

namespace MyGame
{
	class Input
	{
	public:
		static void ScanKey();
		static bool GetKey(int32_t keyCode);
		static bool GetKeyDown(int32_t keyCode);
		static bool GetKeyUp(int32_t keyCode);
	private:
		static SHORT m_oldKeyState[];
		static SHORT m_curKeyState[];
	};
}

#define SCAN_KEY MyGame::Input::ScanKey
#define GET_KEY(key) MyGame::Input::GetKey(key)
#define GET_KEY_DOWN(key) MyGame::Input::GetKeyDown(key)
#define GET_KEY_UP(Key) MyGame::Input::GetKeyUp(Key)
