#pragma once
#include <Windows.h>

#define PI_F 3.14159265358979323846f
#define DEG_TO_RAD 3.1415927f / 180.0f
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
}