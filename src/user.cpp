#include "troglodyte.h"
#include "gui/gui.h"

#include <fstream>

#include <time.h>
#include <stdlib.h>

#include "rect.h"

#include "Sprite.h"
#include "Enemy.h"
#include "Renderer/camera.h"
#include "Make/makeTexture.h"
#include "external/image.h"

#include "external/shaderController.h"

#include <algorithm>

#include "external/time.h"
#include "external/userDefinedStuff.h"

#include "Math/Math.h"

namespace trog
{
	void renderText();

	int maxX = 0;
	int maxY = 0;

	float health = 100.0f;
	float healthSlider = 100.0f;

	bool lastChangeStateI = false;

	bool isPressed = false;
	bool hasPressedSpace = false;

	Enemy* spriteToBeErased;

	int playerLives = 3;

	rect healthRect(133.0f, 15.0f);
	rect innerHealthRect(130.0f, 12.0f);

	rect staminaRect(15.0f, 102.0f);
	rect innerStaminaRect(12.0f, 100.0f);

	float stamina = 100.0f;

	void trog::mainLoop() // ADD DELTA TIME TIMES EVERYTHING
	{
		if (!hasRunOnce)
		{
			isPressed = false;

			srand(time(NULL));

			// Holds the level layout in a list of strings.
			std::vector<std::string> level;

			std::ifstream map_file("../troglodyte/src/map/map.txt", std::ios::in);
			if (map_file.is_open())
			{
				std::string temp;

				std::string line;

				if (std::getline(map_file, temp))
				{
					level.push_back(temp);

					while (std::getline(map_file, line))
					{
						if (line.empty())
							continue;

						//std::cout << line << std::endl;

						level.push_back(line);
					}
				}

				// Parse the level string above.W = wall, E = exit

				int x = 0;
				int y = 0;

				int count = 0;
				bool first_run = true;

				for (std::string row : level)
				{
					//std::cout << row << std::endl;

					for (const auto col : row)
					{
						//std::cout << col << std::endl;
						if (col == 'W')
						{
							//std::cout << "CREATED SPRITE! " << std::endl;

							Sprite* tempSprite = new Sprite(1, "wall");

							tempSprite->position.x = x;
							tempSprite->position.y = y;

							tempSprite->writeToFile = false;
							tempSprite->name = "Unnamed Sprite " + std::to_string(count);

							count += 1;

							//std::cout << tempSprite->position.x << std::endl;
						}

						if (col == 'P')
						{
							//std::cout << "CREATED SPRITE! " << std::endl;

							Sprite* tempSprite = new Sprite(3, "player", Image::playerTexture);

							tempSprite->position.x = x;
							tempSprite->position.y = y;

							tempSprite->writeToFile = false;
							tempSprite->name = "mainPlayer";

							count += 1;
						}

						if (col == 'E')
						{
							//std::cout << "CREATED SPRITE! " << std::endl;

							unsigned int enemyTexture = makeTexture::texture("../troglodyte/res/images/enemyTexture.png");
							Enemy* tempSprite = new Enemy(2, "enemy", enemyTexture);

							tempSprite->position.x = x;
							tempSprite->position.y = y;

							tempSprite->layer = 4;

							tempSprite->writeToFile = false;
							tempSprite->name = "enemy " + std::to_string(count);

							count += 1;
						}

						x += 2;
					}

					if (first_run)
					{
						maxX = x;
						//std::cout << maxX << std::endl;
						first_run = false;
					}
					y -= 2;
					x = 0;
				}

				maxY = y;
			}
			else
			{
				std::cout << "cant open file for map" << std::endl;
			}

			Sprite* mainSprite = GUI::getSpriteName("mainPlayer");

			camera::cameraPos.x = mainSprite->position.x;
			camera::cameraPos.y = mainSprite->position.y;
		}

		Sprite* mainSprite = GUI::getSpriteName("mainPlayer");

		float inputX = inputManager::getAxisInput("horizontal") * 0.4f; //*Time::deltaTime;// *35.0f * Time::deltaTime;
		float inputY = inputManager::getAxisInput("vertical") * 0.4f; //*Time::deltaTime;// *35.0f * Time::deltaTime;

		if (inputManager::keyPressed[GLFW_KEY_SPACE])
		{
			if (isPressed == false)
			{
				float distance = 50.0f;

				spriteToBeErased = NULL;

				for (Enemy*& sprite : enemyList)
				{
					float distanceBetween = funcs::getDistance(mainSprite->position, sprite->position);

					if (distanceBetween < distance)
					{
						distance = distanceBetween;
						spriteToBeErased = sprite;
					}
				}

				hasPressedSpace = true;

				isPressed = true;
			}
		}

		if (!inputManager::keyPressed[GLFW_KEY_SPACE])
		{
			isPressed = false;
		}

		if (hasPressedSpace && stamina >= 80.0f)
		{
			float step = 4.0f;
			try
			{
				if (spriteToBeErased != NULL)
					mainSprite->position = Vector::MoveTowards(mainSprite->position, spriteToBeErased->position, step);
				else
					throw 0;

				inputX = 0;
				inputY = 0;

				if (funcs::getDistance(mainSprite->position, spriteToBeErased->position) <= 1.0f)
				{
					spriteToBeErased->die();

					hasPressedSpace = false;

					stamina -= 80.0f;
				}
			}

			catch (int e)
			{
				// if e == 0, SPRITE == NULL
				stamina -= 80.0f;
			}

			/*if (Sprite::isColliding(mainSprite, spriteToBeErased))
			{
				enemyList.erase(std::remove(enemyList.begin(), enemyList.end(), spriteToBeErased), enemyList.end());
				hasPressedSpace = false;

				inputX = 0;
				inputY = 0;
			}*/

			camera::cameraPos.x = mainSprite->position.x;
			camera::cameraPos.y = mainSprite->position.y;
		}

		if (stamina <= 99.5f)
		{
			stamina += 0.5f;

			if (hasPressedSpace && stamina < 80.0f)
			{
				hasPressedSpace = false;
			}
		}

		//std::cout << stamina << std::endl;

		innerStaminaRect.height = stamina;

		//Image::renderImageFromUI(Image::heartTexture, glm::vec3(-0.75f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), true);

		mainSprite->position.x += inputX;
		mainSprite->position.y += inputY;

		camera::cameraPos.x += inputX;
		camera::cameraPos.y += inputY;

		for (Sprite*& sprite : spriteList)
		{
			if (abs(mainSprite->position.x - sprite->position.x) >= 28 || abs(mainSprite->position.y - sprite->position.y) >= 18)
			{
				sprite->canRender = false;
			}
			else
			{
				sprite->canRender = true;
			}
		}

		for (Enemy*& sprite : enemyList)
		{
			sprite->updateEnemy(mainSprite, innerHealthRect.width);
		}

		float offsetInnerY = (healthRect.height - innerHealthRect.height);
		float offsetInnerX = (healthRect.width - innerHealthRect.width);

		healthRect.renderRectNS(healthRect.width + 9.0f, (SCR_HEIGHT - healthRect.height) - 88.0f, glm::vec3(0.0f, 0.0f, 0.0f));
		innerHealthRect.renderRectNS(healthRect.width - offsetInnerX + 9.0f + 3.0f, (SCR_HEIGHT - innerHealthRect.height) - (88.0f + offsetInnerY), glm::vec3(1.0f, 0.0f, 0.0f));

		staminaRect.renderRectNS(staminaRect.width + 9.0f, (SCR_HEIGHT - staminaRect.height) - 122.0f, glm::vec3(0.0f, 0.0f, 0.0f));
		innerStaminaRect.renderRectNS(innerStaminaRect.width + 9.0f + (staminaRect.width - innerStaminaRect.width), (SCR_HEIGHT - staminaRect.height) - 122.0f + (staminaRect.height - innerStaminaRect.height) - 1.5f, glm::vec3(21.0f / 255.0f, 235.0f / 255.0f, 217.0f / 255.0f));

		//rect::renderRect(glm::vec3(15.0f, SCR_HEIGHT - 150.0f, 0.0f), glm::vec3(300.0f, 40.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), true); // HEALTH CONTAINR

		//rect::renderRect(glm::vec3(21.0f, SCR_HEIGHT - 157.5f, 0.0f), glm::vec3(193.5f + healthSlider , 32.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), true); // HEALTH

		//renderText();
	}

	void renderText()
	{
		std::string healthText = "Health: " + std::to_string(atoi(std::to_string(funcs::playerHealth).c_str()));

		float healthTextSize = fontRenderer::getHeightOfText(healthText, 1.0f);

		fontRenderer::renderText(healthText, 5.0f, SCR_HEIGHT - 125.0f - healthTextSize, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), false, false);
	}
}