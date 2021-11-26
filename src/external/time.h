#pragma once

#include "../troglodyte.h"

namespace trog
{
	class Time
	{
	public:
		static float deltaTime;
		static void calculateDeltaTime();

	private:
		static float lastFrame;
	};
}