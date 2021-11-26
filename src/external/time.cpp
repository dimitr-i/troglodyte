#include "time.h"

namespace trog
{
	float Time::deltaTime = 0.0f;
	float Time::lastFrame = 0.0f;

	void Time::calculateDeltaTime()
	{
		float currentFrame = glfwGetTime();
		deltaTime = (currentFrame - lastFrame);
		lastFrame = currentFrame;
	}
}