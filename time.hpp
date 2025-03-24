#pragma once
#include <Windows.h>

namespace MyGame
{
	class Time
	{
	public:
		static void Initialize();
		static void UpdateTime();

		static float GetDeltaTime() { return m_deltaTime; }
		static float GetTotalTime() { return m_totalTime; }

	private:
		static LARGE_INTEGER m_frequency;
		static LARGE_INTEGER m_prevCounter;
		static LARGE_INTEGER m_currentCounter;
		static LARGE_INTEGER m_initCounter;

		static float m_deltaTime;
		static float m_totalTime;
	};
}

#define UPDATE_TIME MyGame::Time::UpdateTime
#define GET_DELTATIME MyGame::Time::GetDeltaTime
#define GET_TOTALTIME MyGame::Time::GetTotalTime
