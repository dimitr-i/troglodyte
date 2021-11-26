#include "camera.h"
#include "renderer.h"
#include "../gui/gui.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp> 

#define _ITERATOR_DEBUG_LEVEL 0

#include "../external/time.h"

namespace trog
{
	float firstMouse = true;

	glm::vec3 camera::cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 camera::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 camera::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 camera::cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

	float camera::fov = 90.0f;

	float camera::yaw = -45.0f;
	float camera::pitch = 0.0f;

	float camera::lastX = 0.0f;
	float camera::lastY = 0.0f;

	glm::mat4 camera::view = glm::mat4(1.0f);
	glm::mat4 camera::projection = glm::perspective(glm::radians(fov), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

	float camera::mouseSensitivity = 55.0f;

	bool camera::canShowCursor = false;

	void camera::update()
	{
		float cameraSpeed = 5.0f * Time::deltaTime;

		if (canTakeGameInput)
		{
			if (inputManager::keyPressed[GLFW_KEY_LEFT])
			{
				yaw += -2.0f;
			}

			if (inputManager::keyPressed[GLFW_KEY_RIGHT])
			{
				yaw += 2.0f;
			}

			if (inputManager::keyPressed[GLFW_KEY_UP])
			{
				pitch += 2.0f;
			}

			if (inputManager::keyPressed[GLFW_KEY_DOWN])
			{
				pitch += -2.0f;
			}

			if (inputManager::keyPressed[GLFW_KEY_P])
			{
				canShowCursor = !canShowCursor;
			}

			if (canShowCursor)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			/* FPS CAMERA */
			if (isInPlayMode)
			{
				if (inputManager::keyPressed['A'])
				{
					cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
				}

				if (inputManager::keyPressed['D'])
				{
					cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
				}

				if (inputManager::keyPressed['W'])
				{
					cameraPos += cameraFront * cameraSpeed;
				}

				if (inputManager::keyPressed['S'])
				{
					cameraPos -= cameraFront * cameraSpeed;
				}

				if (inputManager::keyPressed[GLFW_KEY_SPACE])
				{
					cameraPos.y += 2.0f;
				}

				if (inputManager::keyPressed[GLFW_KEY_LEFT_SHIFT])
				{
					cameraPos.y -= 2.0f;
				}

				if (inputManager::keyPressed[GLFW_KEY_LEFT_CONTROL])
				{
					cameraSpeed = 4.0f * Time::deltaTime;
				}

				cameraPos.y = 0.0f;

				glm::vec2 mouseCoords = renderer::getMouseCoords();

				if (firstMouse)
				{
					lastX = mouseCoords.x;
					lastY = mouseCoords.y;
					firstMouse = false;
				}

				float xoffset = mouseCoords.x - lastX;
				float yoffset = lastY - mouseCoords.y;

				xoffset *= mouseSensitivity;
				yoffset *= mouseSensitivity;

				//yaw += xoffset;
				//pitch += yoffset;

				// Make sure that when pitch is out of bounds, screen doesn't get flipped
				if (pitch > MAX_LOOK_ANGLE)
				{
					pitch = MAX_LOOK_ANGLE;
				}
				if (pitch < -MAX_LOOK_ANGLE)
				{
					pitch = -MAX_LOOK_ANGLE;
				}

				lastX = mouseCoords.x;
				lastY = mouseCoords.y;

				if (inputManager::keyPressed[GLFW_KEY_P])
				{
					canShowCursor = !canShowCursor;
				}
				if (canShowCursor)
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}

			else
			{
				if (inputManager::mousePressed[2])
				{
					glm::vec2 mouseCoords = renderer::getMouseCoords();

					float xoffset = mouseCoords.x - inputManager::middleMousePressedDownPos.x;
					float yoffset = inputManager::middleMousePressedDownPos.y - mouseCoords.y;

					if (yoffset > 0.0f)
					{
						cameraPos -= cameraUp * (cameraSpeed + 1);
					}

					else if (yoffset < 0.0f)
					{
						cameraPos += cameraUp * (cameraSpeed + 1);
					}

					if (xoffset > 0.0f)
					{
						cameraPos -= cameraRight * (cameraSpeed + 1);
					}

					else if (xoffset < 0.0f)
					{
						cameraPos += cameraRight * (cameraSpeed + 1);
					}

					inputManager::middleMousePressedDownPos = mouseCoords;
				}
				if (camera::canShowCursor == true)
				{
					glm::vec2 mouseCoords = renderer::getMouseCoords();

					float xoffset = mouseCoords.x - inputManager::mousePressedDownPos.x;
					float yoffset = inputManager::mousePressedDownPos.y - mouseCoords.y;
					inputManager::mousePressedDownPos = mouseCoords;

					xoffset *= mouseSensitivity;
					yoffset *= mouseSensitivity;

					//yaw += xoffset;
					//pitch += yoffset;

					// Make sure that when pitch is out of bounds, screen doesn't get flipped
					if (pitch > MAX_LOOK_ANGLE)
					{
						pitch = MAX_LOOK_ANGLE;
					}
					if (pitch < -MAX_LOOK_ANGLE)
					{
						pitch = -MAX_LOOK_ANGLE;
					}
				}
				else
				{
					if (inputManager::mousePressed[1])
					{
						glm::vec2 mouseCoords = renderer::getMouseCoords();

						float xoffset = mouseCoords.x - inputManager::mousePressedDownPos.x;
						float yoffset = inputManager::mousePressedDownPos.y - mouseCoords.y;
						inputManager::mousePressedDownPos = mouseCoords;

						xoffset *= mouseSensitivity;
						yoffset *= mouseSensitivity;

						//yaw += xoffset;
						//pitch += yoffset;

						// Make sure that when pitch is out of bounds, screen doesn't get flipped
						if (pitch > MAX_LOOK_ANGLE)
						{
							pitch = MAX_LOOK_ANGLE;
						}
						if (pitch < -MAX_LOOK_ANGLE)
						{
							pitch = -MAX_LOOK_ANGLE;
						}
					}
				}
			}

			updateVectors();
		}

		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void camera::updateVectors()
	{
		//std::cout << glm::to_string(cameraPos) << std::endl;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);

		//cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	glm::mat4 camera::getProjection()
	{
		//projection = glm::perspective(glm::radians(fov), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
		projection = glm::ortho(-24.0f, 24.0f, -16.0f, 16.0f, 0.1f, 100.0f);

		return projection;
	}

	glm::mat4 camera::getView()
	{
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		return view;
	}
}