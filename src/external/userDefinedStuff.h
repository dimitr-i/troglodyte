#pragma once

#include "../troglodyte.h"
#include "../Math/Math.h"

namespace trog
{
	class funcs
	{
	public:
		static float playerHealth;
		static float playerRange;

		static void loseHealth(float &width);
		static float getDistance(glm::vec3 player, glm::vec3 enemy);

	private:
		static int counterForPlayerDamage;
	};
}