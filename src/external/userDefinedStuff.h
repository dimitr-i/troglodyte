#pragma once

#include "../troglodyte.h"

namespace trog
{
	class funcs
	{
	public:
		static float playerHealth;

		static void loseHealth(float &width);
		static float getDistance(glm::vec3 player, glm::vec3 enemy);
	};
}