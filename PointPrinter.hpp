#pragma once
#include <vector>

namespace MyGame
{
	struct PointNode
	{
		const wchar_t* point;
		float time;
		float posX;
		float posY;
		float velX;
		float velY;

		PointNode(const wchar_t* p, float t, float x, float y, float vx, float vy)
			: time(t), posX(x), posY(y), velX(vx), velY(vy)
		{
			point = p;
		}
	};

	class PointPrinter
	{
	public: 
		void CreatePoint(const wchar_t* point,float time,
			float posX, float posY, 
			float velX, float velY);

		void UpdatePoints();

		void ClearAllPoints() { m_points.clear(); }

		const std::vector<PointNode>* GetPoints() const { return &m_points; }

	private:
		std::vector<PointNode> m_points;
	};
}
