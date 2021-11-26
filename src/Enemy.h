#pragma once

#include "troglodyte.h"
#include "Sprite.h"
#include "Math/Math.h"

namespace trog
{
	class Enemy : public Sprite
	{
	public:
		Enemy(int LAYER, std::string type, unsigned int image);

		void attack(float &innerRectWidth);

		void updateEnemy(Sprite*& mainSprite, float &innerRectWidth);

		void defend();
		void take_damage(float amount);
		void renderHealthBar();
		void renderRect(glm::vec3 position_, glm::vec3 size_, glm::vec3 rotate_, glm::vec3 color_);

		void die();

	private:
		const float attack_range = 3.0f;
		const float sight_range = 12.0f;

		float enemy_health = 25.0f;

		float containerWidth = 0.75f;
		float containerHeight = 0.25f;

		float innerWidth = 0.70f;
		float innerHeight = 0.20f;

		bool enemyCanTakeDamage = true;
	};
}