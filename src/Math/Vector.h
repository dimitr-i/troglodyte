#pragma once

#include "../troglodyte.h"

namespace trog
{
	class Vector
	{
	public:
		static glm::vec3 MoveTowards(glm::vec3 current, glm::vec3 target, float maxDistanceDelta);
	};
}