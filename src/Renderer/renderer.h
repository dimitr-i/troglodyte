#pragma once

#include "../troglodyte.h"

namespace trog
{
	class renderer
	{
	public:
		static std::string GetFPS();

		static glm::vec2 getMouseCoords();

		static GLFWwindow* windowSetup();
	};
}
