#include "time.h"

namespace trog
{
	float time_::deltaTime = 0.0f;
	float time_::lastFrame = 0.0f;

	void time_::calculateDeltaTime()
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
}