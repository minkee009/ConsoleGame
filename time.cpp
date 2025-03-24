#include "time.hpp"

LARGE_INTEGER MyGame::Time::m_frequency;
LARGE_INTEGER MyGame::Time::m_prevCounter;
LARGE_INTEGER MyGame::Time::m_currentCounter;
LARGE_INTEGER MyGame::Time::m_initCounter;

float MyGame::Time::m_deltaTime;
float MyGame::Time::m_totalTime;

void MyGame::Time::Initialize()
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_initCounter);
}

void MyGame::Time::UpdateTime()
{
	QueryPerformanceCounter(&m_currentCounter);
	m_deltaTime = static_cast<float>(m_currentCounter.QuadPart - m_prevCounter.QuadPart) / m_frequency.QuadPart;
	m_totalTime = static_cast<float>(m_currentCounter.QuadPart - m_initCounter.QuadPart) / m_frequency.QuadPart;
	m_prevCounter = m_currentCounter;
}
