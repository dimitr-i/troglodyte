#include "Enemy.h"
#include "external/image.h"
#include "external/userDefinedStuff.h"

namespace trog
{
	Enemy::Enemy(int LAYER, std::string type, unsigned int image)
	{
		enemyList.emplace_back(this);

		if (std::find(spriteList.begin(), spriteList.end(), this) != spriteList.end())
		{
			spriteList.erase(std::remove(spriteList.begin(), spriteList.end(), this), spriteList.end());
		}

		showInEditor = false;
		writeToFile = false;
		texture = image;

		layer = LAYER;
		//texture = Image::playButtonTexture;
	}

	void Enemy::attack()
	{

	}

	void Enemy::updateEnemy(Sprite*& mainSprite, float &innerRectWidth)
	{


		if (funcs::getDistance(mainSprite->position, this->position) <= 10.0f)
		{
			if (funcs::getDistance(mainSprite->position, this->position) <= 2.0f)
			{
				funcs::loseHealth(innerRectWidth);
			}

			if (3 <= 3) // rand() % 20
			{
				this->position = MoveTowards(this->position, mainSprite->position, 0.25f); // 8.0f * time_::deltaTime);
			}
		}
	}

	glm::vec3 Enemy::MoveTowards(glm::vec3 current, glm::vec3 target, float maxDistanceDelta) //UNITY SOURCE CODE
	{
		glm::vec3 a = target - current;
		float magnitude = glm::length(a);
		if (magnitude <= maxDistanceDelta || magnitude == 0.0f)
		{
			return target;
		}
		return current + a / magnitude * maxDistanceDelta;
	}
}