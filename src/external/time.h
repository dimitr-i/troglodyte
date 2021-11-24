#pragma once

#include "../troglodyte.h"

namespace trog
{
	class time_
	{
	public:
		static float deltaTime;
		static void calculateDeltaTime();

	private:
		static float lastFrame;
	};
}