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
			*posX -= penetrationX; // 왼쪽으로 밀기
			if (abs(*posX - prevPosX) >= 0.000001)
				*posX = round(*posX);

			return MATH_COL_FLAG_PUSHLEFT;
		}
		else
		{
			*posX += penetrationX; // 오른쪽으로 밀기
			return MATH_COL_FLAG_PUSHRIGHT;
		}
	}
	else {
		if (minY < minY2)
		{
			*posY = minY2 - ceil(maxY - minY);// 위로 밀기
			return MATH_COL_FLAG_PUSHUP;
		}
		else
		{
			*posY += penetrationY; // 아래로 밀기
			return MATH_COL_FLAG_PUSHDOWN;
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
			return MATH_COL_FLAG_PUSHLEFT;
		else
			return MATH_COL_FLAG_PUSHRIGHT;
	}
	else {
		if (minY < minY2)
		{
			return MATH_COL_FLAG_PUSHUP;
		}
		else
		{
			return MATH_COL_FLAG_PUSHDOWN;
		}
	}
	return 0;
}

bool MyGame::CheckAABB(Bbox a, Bbox b)
{
	return (a.maxX >= b.minX && a.minX <= b.maxX && a.maxY >= b.minY && a.minY <= b.maxY);
}

int MyGame::ApplyPenetration(float* posX, float* posY, Bbox a, Bbox b)
{
	float penetrationX = min(a.maxX - b.minX, b.maxX - a.minX);
	float penetrationY = min(a.maxY - b.minY, b.maxY - a.minY);

	if (penetrationX < penetrationY) {
		if (a.minX < b.minX)
		{
			auto prevPosX = *posX;
			*posX -= penetrationX; // 왼쪽으로 밀기
			if (abs(*posX - prevPosX) >= 0.000001)
				*posX = round(*posX);

			return MATH_COL_FLAG_PUSHLEFT;
		}

		else
		{
			*posX += penetrationX; // 오른쪽으로 밀기
			return MATH_COL_FLAG_PUSHRIGHT;
		}
	}
	else {
		if (a.minY < b.minY)
		{
			*posY = b.minY - ceil(a.maxY - a.minY);// 위로 밀기
			return MATH_COL_FLAG_PUSHUP;
		}
		else
		{
			*posY += penetrationY; // 아래로 밀기
			return MATH_COL_FLAG_PUSHDOWN;
		}
	}
	return 0;
}

int MyGame::CalcPenetration(Bbox a, Bbox b)
{
	float penetrationX = min(a.maxX - b.minX, b.maxX - a.minX);
	float penetrationY = min(a.maxY - b.minY, b.maxY - a.minY);

	if (penetrationX < penetrationY) {
		if (a.minX < b.minX)
			return MATH_COL_FLAG_PUSHLEFT;
		else
			return MATH_COL_FLAG_PUSHRIGHT;
	}
	else {
		if (a.minY < b.minY)
		{
			return MATH_COL_FLAG_PUSHUP;
		}
		else
		{
			return MATH_COL_FLAG_PUSHDOWN;
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

float MyGame::lerp_s_f(float a, float b, float t)
{
	return a + (b - a) * t;
}
