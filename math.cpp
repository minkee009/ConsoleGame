#include "math.hpp"
#include <cmath>
#include <Windows.h>

bool MyGame::CheckAABB(float maxX, float maxY, float minX, float minY,
	float maxX2, float maxY2, float minX2, float minY2)
{
	return (maxX >= minX2 && minX <= maxX2 && maxY >= minY2 && minY <= maxY2);
}

int MyGame::ApplyPenetration(float* posX, float* posY,
	float maxX, float maxY, float minX, float minY,
	float maxX2, float maxY2, float minX2, float minY2)
{
	float penetrationX = min(maxX - minX2, maxX2 - minX);
	float penetrationY = min(maxY - minY2, maxY2 - minY);
	if (penetrationX < penetrationY) {
		if (minX < minX2)
		{
			auto prevPosX = *posX;
			*posX -= penetrationX; // �������� �б�
			if (abs(*posX - prevPosX) >= 0.000001)
				*posX = round(*posX);

			return 1;
		}
		else
		{
			*posX += penetrationX; // ���������� �б�
			return 2;
		}
	}
	else {
		if (minY < minY2)
		{
			*posY = minY2 - ceil(maxY - minY);// ���� �б�
			return 3;
		}
		else
		{
			*posY += penetrationY; // �Ʒ��� �б�
			return 4;
		}
	}
	return 0;
}

int MyGame::CalcPenetration(float maxX, float maxY, float minX, float minY, float maxX2, float maxY2, float minX2, float minY2)
{
	float penetrationX = min(maxX - minX2, maxX2 - minX);
	float penetrationY = min(maxY - minY2, maxY2 - minY);

	if (penetrationX < penetrationY) {
		if (minX < minX2)
			return 1;
		else
			return 2;
	}
	else {
		if (minY < minY2)
		{
			return 3;
		}
		else
		{
			return 4;
		}
	}
	return 0;
}



float MyGame::Distance(float ax, float ay, float bx, float by)
{
	return sqrt(SqrtDistance(ax,ay,bx,by));
}

float MyGame::SqrtDistance(float ax, float ay, float bx, float by)
{
	float absX = abs(ax - bx);
	float absY = abs(ay - by);

	absX *= absX;
	absY *= absY;
	return absX + absY;
}