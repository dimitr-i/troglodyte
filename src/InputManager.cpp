#include "InputManager.h"
#include "gui/gui.h"
#include "Renderer/camera.h"

namespace trog
{
	bool inputManager::keyPressed[KEYS];
	bool inputManager::mousePressed[10];

	bool inputManager::isMiddleMousePressedDown = false;
	glm::vec2 inputManager::middleMousePressedDownPos = glm::vec2(0.0f, 0.0f);
	glm::vec2 inputManager::mousePressedDownPos = glm::vec2(0.0f, 0.0f);

	int inputManager::lastX = 0;
	int inputManager::lastY = 0;

	bool inputManager::firstMouse = true;

	void inputManager::mouse_click_callback(GLFWwindow* win, int button, int action, int mods)
	{
		inputManager* input = (inputManager*)glfwGetWindowUserPointer(win);
		input->mouse(win, button, action, mods);
	}

	void inputManager::mouse(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS)
			inputManager::mousePressed[button] = true;

		if (action == GLFW_RELEASE)
			inputManager::mousePressed[button] = false;
	}

	void inputManager::key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
		inputManager* input = (inputManager*)glfwGetWindowUserPointer(win);
		input->keys(win, key, scancode, action, mods);
	}

	void inputManager::keys(GLFWwindow* win, int key, int scancode, int action, int mods) {
		if (canTakeGameInput)
		{
			if (key == GLFW_KEY_UNKNOWN) return; // Don't accept unknown keys

			if (action == GLFW_PRESS)
			{
				inputManager::keyPressed[key] = true;
				//std::cout << key << std::endl;
			}

			else if (action == GLFW_RELEASE)
				inputManager::keyPressed[key] = false;

			getError();
		}

		if (key == GLFW_KEY_N && mods == GLFW_MOD_CONTROL) // control n
		{
			if (GUI::canMakeNewSprite)
				GUI::createNewSprite(false);
		}

		if (key == GLFW_KEY_N && mods == 3) // control shift n
		{
			if (GUI::canMakeNewSprite)
			{
				GUI::createNewSprite(true);
			}
		}

		if (key == GLFW_KEY_C && mods == 3)
		{
			camera::cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
			camera::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			camera::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			camera::cameraRight = glm::normalize(glm::cross(camera::cameraFront, camera::cameraUp));

			camera::fov = 160.0f;

			camera::yaw = -90.0f;
			camera::pitch = 0.0f;
		}

		if (key == GLFW_KEY_P && mods == GLFW_MOD_SHIFT)
		{
			isInPlayMode = false;
		}

		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS)
				glfwSetWindowShouldClose(win, true);
		}
	}

	void inputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (canTakeGameInput)
		{
			camera::fov -= (float)yoffset;
			if (camera::fov < 100.0f)
				camera::fov = 100.0f;
			if (camera::fov > 160.0f)
				camera::fov = 160.0f;

			//std::cout << camera::fov << std::endl;
		}
	}

	int inputManager::getAxisInput(const char* axis)
	{
		inputManager man;

		if (canTakeGameInput)
		{
			if (axis == "horizontal")
			{
				if ((glfwGetKey(window, GLFW_KEY_LEFT) || glfwGetKey(window, GLFW_KEY_A)) == GLFW_PRESS)
					man.xLeftPressed = true;

				if ((glfwGetKey(window, GLFW_KEY_LEFT) || glfwGetKey(window, GLFW_KEY_A)) == GLFW_RELEASE)
					man.xLeftPressed = false;

				if ((glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D)) == GLFW_PRESS)
					man.xRightPressed = true;

				if ((glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D)) == GLFW_RELEASE)
					man.xRightPressed = false;

				if (man.xLeftPressed == true && man.xRightPressed == true)
					return 0;

				if (man.xLeftPressed == true)
					return -1;

				if (man.xRightPressed == true)
					return 1;

				if (man.xLeftPressed == false && man.xRightPressed == false)
					return 0;
			}

			if (axis == "vertical")
			{
				if ((glfwGetKey(window, GLFW_KEY_UP) || glfwGetKey(window, GLFW_KEY_W)) == GLFW_PRESS)
					man.yUpPressed = true;

				if ((glfwGetKey(window, GLFW_KEY_UP) || glfwGetKey(window, GLFW_KEY_W)) == GLFW_RELEASE)
					man.yUpPressed = false;

				if ((glfwGetKey(window, GLFW_KEY_DOWN) || glfwGetKey(window, GLFW_KEY_S)) == GLFW_PRESS)
					man.yDownPressed= true;

				if ((glfwGetKey(window, GLFW_KEY_DOWN) || glfwGetKey(window, GLFW_KEY_S)) == GLFW_RELEASE)
					man.yDownPressed= false;

				if (man.yUpPressed == true && man.yDownPressed == true)
					return 0;

				if (man.yUpPressed == true)
					return 1;

				if (man.yDownPressed == true)
					return -1;

				if (man.yUpPressed == false && man.yDownPressed == false)
					return 0;
			}
		}
	}

	glm::vec2 inputManager::getMouseCoords()
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return glm::vec2(xpos, ypos);
	}
}
