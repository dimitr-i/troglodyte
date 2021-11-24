#pragma once

#include "troglodyte.h"

namespace trog
{
	class rect
	{
	public:
		rect(float _width, float _height);

		float x;
		float y;

		float width;
		float height;

		static void renderRect(glm::vec3 position, glm::vec3 size, glm::vec3 rotate, glm::vec3 color, bool fromCenter);
		void renderRectNS(float _x, float _y, glm::vec3 color);
	};
}