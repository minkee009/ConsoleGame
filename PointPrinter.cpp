#include "PointPrinter.hpp"
#include "engine.hpp"

void MyGame::PointPrinter::CreatePoint(const wchar_t* point, float time, float posX, float posY, float velX, float velY)
{
	m_points.emplace_back(point,time,posX,posY,velX,velY);
}

void MyGame::PointPrinter::UpdatePoints()
{
	for (auto it = m_points.begin(); it != m_points.end();)
	{
		it->time -= GET_DELTATIME();
		if (it->time < 0.0f)
		{
			it = m_points.erase(it);
		}
		else
		{
			it->posX += it->velX * GET_DELTATIME();
			it->posY += it->velY * GET_DELTATIME();
			++it;
		}
	}
}
