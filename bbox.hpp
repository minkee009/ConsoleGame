#pragma once

namespace MyGame
{
	struct Bbox
	{
		float maxX;
		float maxY;
		float minX;
		float minY;

		Bbox(float maxX, float maxY, float minX, float minY)
			: maxX(maxX), maxY(maxY), minX(minX), minY(minY) {
		}
	};
}