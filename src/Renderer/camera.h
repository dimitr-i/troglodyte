#pragma once
#include "../troglodyte.h"

namespace trog
{
	class camera
	{
	public:
		static glm::mat4 getView();
		static glm::mat4 getProjection();
		static void update();

		static glm::vec3 cameraPos;
		static glm::vec3 cameraFront;
		static glm::vec3 cameraUp;
		static glm::vec3 cameraRight;

		static float fov;

		static float yaw;
		static float pitch;

		static bool canShowCursor;
	private:
		static float lastX;
		static float lastY;

		static glm::mat4 view;
		static glm::mat4 projection;

		static float mouseSensitivity;
		static void updateVectors();
	};
}