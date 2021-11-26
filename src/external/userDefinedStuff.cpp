#include "userDefinedStuff.h"

namespace trog
{
	float funcs::playerHealth = 100.0f;
	float funcs::playerRange = 4.0f;

	int funcs::counterForPlayerDamage = 0;

	void funcs::loseHealth(float &width)
	{
		//std::cout << counterForPlayerDamage << std::endl;

		counterForPlayerDamage += 1;

		if (counterForPlayerDamage >= 30)
		{
			playerHealth -= 10.0f;
			if (width >= 13.0f)
				width -= 13.0f;

			counterForPlayerDamage = 0;
		}
	}

	float funcs::getDistance(glm::vec3 player, glm::vec3 enemy)
	{
		float distanceX = abs(player.x - enemy.x);
		float distanceY = abs(player.y - enemy.y);

		return Math::hypo(distanceX, distanceY);
	}
}