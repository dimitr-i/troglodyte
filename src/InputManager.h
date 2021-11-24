#pragma once

#include "troglodyte.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace trog
{
	class inputManager
	{
	private:
		bool xLeftPressed = false;
		bool xRightPressed = false;
		bool yUpPressed = false;
		bool yDownPressed = false;

		int old_state = 0;
		int new_state = 0;

		static const int KEYS = 349;
	public:
		static int getAxisInput(const char* axis);
		static void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);
		static void keys(GLFWwindow* win, int key, int scancode, int action, int mods);

		static void mouse_click_callback(GLFWwindow* win, int button, int action, int mods);
		static void mouse(GLFWwindow* window, int button, int action, int mods);

		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		static glm::vec2 getMouseCoords();

		static bool keyPressed[KEYS];
		static bool mousePressed[10];

		static bool isMiddleMousePressedDown;
		static glm::vec2 middleMousePressedDownPos;
		static glm::vec2 mousePressedDownPos;

		static int lastX;
		static int lastY;

		static bool firstMouse;
	};
}
