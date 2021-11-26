#include "Vector.h"

namespace trog
{
	glm::vec3 Vector::MoveTowards(glm::vec3 current, glm::vec3 target, float maxDistanceDelta)
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