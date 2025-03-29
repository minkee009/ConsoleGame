#pragma once
#include <Windows.h>

namespace MyGame
{
	bool CheckAABB(float maxX, float maxY, float minX, float minY,
		float maxX2, float maxY2, float minX2, float minY2);

	int ApplyPenetration(float* posX, float* posY,
		float maxX, float maxY, float minX, float minY,
		float maxX2, float maxY2, float minX2, float minY2);

	int CalcPenetration(float maxX, float maxY, float minX, float minY,
		float maxX2, float maxY2, float minX2, float minY2);

	float Distance(float ax, float ay, float bx, float by);

	float SqrtDistance(float ax, float ay, float bx, float by);

	bool CheckDistance(COORD center, float xRange, float yRange, float posX, float posY);
}