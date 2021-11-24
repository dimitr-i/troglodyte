#include "userDefinedStuff.h"

namespace trog
{
	float funcs::playerHealth = 100.0f;

	void funcs::loseHealth(float &width)
	{
		playerHealth -= 10.0f;
		if (width >= 13.0f)
			width -= 13.0f;
	}

	float funcs::getDistance(glm::vec3 player, glm::vec3 enemy)
	{
		float distanceX = abs(player.x - enemy.x);
		float distanceY = abs(player.y - enemy.y);

		float result = pow(distanceX, 2) + pow(distanceY, 2);

		return sqrt(result);
	}
}