#include "Renderer.h"
#include "../Math/Math.h"
#include <chrono>
#include <string>

namespace trog
{
	glm::vec2 renderer::getMouseCoords()
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		xpos = xpos / 800 - 1;
		ypos = ypos / 600 - 1;
		return glm::vec2(xpos, ypos);
	}

	std::string renderer::GetFPS()
	{
		static std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();
		static int fps; fps++;

		if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{ 1 }) {
			oldTime = std::chrono::high_resolution_clock::now();
			std::cout << "FPS: " << fps << std::endl;
			fps = 0;
		}

		return std::to_string(fps); //FIX THIS
	}

	GLFWwindow* renderer::windowSetup()
	{
		if (!glfwInit())
			std::cout << "GLFW INITIALIZING ERROR" << std::endl;

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif // __APPLE__


		GLFWwindow* temp_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "trog", NULL, NULL);

		glfwMakeContextCurrent(temp_window);

		glewExperimental = GL_TRUE;

		GLenum glewError = glewInit();

		if (glewError != GLEW_OK)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwSwapInterval(0);

		trog::getError();

		return temp_window;
	}
}
