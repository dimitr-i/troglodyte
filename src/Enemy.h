#pragma once

#include "troglodyte.h"
#include "Sprite.h"

namespace trog
{
	class Enemy : public Sprite
	{
	public:
		Enemy(int LAYER, std::string type, unsigned int image);

		void attack();

		void updateEnemy(Sprite*& mainSprite, float &innerRectWidth);

		glm::vec3 MoveTowards(glm::vec3 current, glm::vec3 target, float maxDistanceDelta);

		void defend();

		void die();
	};
}