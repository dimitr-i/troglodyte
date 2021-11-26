#include "Enemy.h"
#include "external/image.h"
#include "external/userDefinedStuff.h"
#include "external/time.h"

#include "Renderer/camera.h"

#include "external/shaderController.h"
#include "external/Image.h"

namespace trog
{
	Enemy::Enemy(int LAYER, std::string type, unsigned int image)
	{
		enemyList.emplace_back(this);

		if (std::find(spriteList.begin(), spriteList.end(), this) != spriteList.end())
		{
			spriteList.erase(std::remove(spriteList.begin(), spriteList.end(), this), spriteList.end());
		}

		showInEditor = false;
		writeToFile = false;
		texture = image;

		layer = LAYER;
		//texture = Image::playButtonTexture;
	}

	void Enemy::take_damage(float amount)
	{
		Enemy::enemy_health -= amount;
		innerWidth -= 0.7f / 3.0f;

		if (enemy_health <= 0)
		{
			die();
		}
	}

	void Enemy::renderHealthBar()
	{
		renderRect(glm::vec3(position.x, position.y + size.y + containerHeight * 2, 0.0f), glm::vec3(containerWidth, containerHeight, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		//healthRect.renderRectNS(healthRect.width + 9.0f, (SCR_HEIGHT - healthRect.height) - 88.0f, glm::vec3(0.0f, 0.0f, 0.0f));
		//innerHealthRect.renderRectNS(healthRect.width - offsetInnerX + 9.0f + 3.0f, (SCR_HEIGHT - innerHealthRect.height) - (88.0f + offsetInnerY), glm::vec3(1.0f, 0.0f, 0.0f));

		renderRect(glm::vec3(position.x - (containerWidth - innerWidth) + 0.05f, position.y + size.y + innerHeight * 2 + ((containerHeight - innerHeight) * 2), 0.0f), glm::vec3(innerWidth, innerHeight, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	void Enemy::die()
	{
		enemyList.erase(std::remove(enemyList.begin(), enemyList.end(), this), enemyList.end());
		delete this;
		//drop_inventory();
	}

	void Enemy::attack(float &innerRectWidth)
	{
		funcs::loseHealth(innerRectWidth);
	}

	void Enemy::updateEnemy(Sprite*& mainSprite, float &innerRectWidth)
	{
		renderHealthBar();

		if (funcs::getDistance(mainSprite->position, this->position) <= sight_range)
		{
			if ((funcs::getDistance(mainSprite->position, this->position) <= funcs::playerRange) && inputManager::keyPressed[GLFW_KEY_E] && enemyCanTakeDamage) // TEMPORARY ATTACKING
			{
				take_damage(10.0f);
				enemyCanTakeDamage = false;
			}

			if (!inputManager::keyPressed[GLFW_KEY_E])
			{
				enemyCanTakeDamage = true;
			}

			if (funcs::getDistance(mainSprite->position, this->position) <= attack_range)
			{
				attack(innerRectWidth);
			}

			this->position = Vector::MoveTowards(this->position, mainSprite->position, 5.0f * Time::deltaTime); // 8.0f * Time::deltaTime);
		}
	}

	void Enemy::renderRect(glm::vec3 position_, glm::vec3 size_, glm::vec3 rotate_, glm::vec3 color_)
	{
		//std::cout << shape << std::endl;
		if (is3d)
		{
			glm::mat4 projection = camera::getProjection();
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = camera::getView();

			//position.x = position.x / SCR_WIDTH - 1.0f;
			//position.y = position.y / SCR_HEIGHT - 1.0f;

			model = glm::translate(model, position_);

			model = glm::translate(model, glm::vec3(0.5f * size_.x, 0.5f * size_.y, 0.5f * size_.z));

			model = glm::rotate(model, glm::radians(rotate_.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate_.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate_.z), glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::translate(model, glm::vec3(-0.5f * size_.x, -0.5f * size_.y, -0.5f * size_.z));

			model = glm::scale(model, size_);

			glm::mat4 mvp = projection * view * model;

			glUseProgram(shaderProgram);

			int loc = shaderController::getUniform("u_Color");

			if (loc != -1)
			{
				shaderController::uniform(loc, color_.x, color_.y, color_.z);
				//std::cout << color[0] << " : " << color[1] << " : " << color[2] << std::endl;
			}
			else
				std::cout << "CANT FIND COLOR" << std::endl;

			int mvploc = shaderController::getUniform("mvp");

			if (mvploc != -1)
				glUniformMatrix4fv(mvploc, 1, GL_FALSE, glm::value_ptr(mvp));
			else
				std::cout << "CANT FIND MVGP" << std::endl;

			glUseProgram(0);
		}

		shaderController::texture(0);

		glUseProgram(shaderProgram);

		glBindVertexArray(vao);

		std::vector<float> fullData;

		int textureCounter = 0;

		for (int i = 0; i < vertices.size(); i = i + 3)
		{
			fullData.push_back(vertices.at(i));
			fullData.push_back(vertices.at(i + 1));
			fullData.push_back(vertices.at(i + 2));

			fullData.push_back(textures.at(textureCounter));
			fullData.push_back(textures.at(1 + textureCounter));

			textureCounter += 2;
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		glBufferData(GL_ARRAY_BUFFER, fullData.size() * sizeof(float), &fullData.front(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		getError("UPDATE SPRITE");
		glActiveTexture(GL_TEXTURE0);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);

		getError();
	}
}