#include "Sprite.h"
#include <string>
#include "Renderer/camera.h"

#include "external/shaderController.h"
#include "external/Image.h"

using namespace shaderController;

namespace trog
{
	Sprite::Sprite(int LAYER, std::string type, unsigned int image)
	{
		if (type == "player" || type == "wall")
			spriteList.emplace_back(this);

		showInEditor = false;
		writeToFile = false;
		texture = image;

		layer = LAYER;
		//texture = Image::playButtonTexture;

		update();
	}

	void Sprite::Update()
	{
		//std::cout << "BASE CLASS SPRITE" << std::endl;
		;
		//this means that there is no file to override this function
	}

	void Sprite::renderSprite()
	{
		if (texture != 0)
		{
			hasTextureActive = true;
		}

		//std::cout << shape << std::endl;
		if (is3d)
		{
			glm::mat4 projection = camera::getProjection();
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = camera::getView();

			//position.x = position.x / SCR_WIDTH - 1.0f;
			//position.y = position.y / SCR_HEIGHT - 1.0f;

			model = glm::translate(model, position);

			model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));

			model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z));

			model = glm::scale(model, size);

			glm::mat4 mvp = projection * view * model;

			glUseProgram(shaderProgram);
			
			int loc = getUniform("u_Color");
			
			if (loc != -1)
			{
				shaderController::uniform(loc, color[0], color[1], color[2]);
				//std::cout << color[0] << " : " << color[1] << " : " << color[2] << std::endl;
			}
			else
				std::cout << "CANT FIND COLOR" << std::endl;

			int mvploc = getUniform("mvp");

			if (mvploc != -1)
				glUniformMatrix4fv(mvploc, 1, GL_FALSE, glm::value_ptr(mvp));
			else
				std::cout << "CANT FIND MVGP" << std::endl;

			glUseProgram(0);
		}

		/*else
		{
			//glm::mat4 projection = renderer.getProjection(is3d);
			glm::mat4 projection = glm::ortho(-SCR_WIDTH / 2.0f, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f, -SCR_HEIGHT / 2.0f, -1.0f, 100.0f);
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = camera::getView();

			model = glm::translate(model, position);
			model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, size);

			//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

			glm::mat4 mp = projection * model;

			glUseProgram(shaderProgram);

			int loc = glGetUniformLocation(shaderProgram, "u_Color");

			if (loc != -1)
				glUniform3f(loc, color[0], color[1], color[2]);

			//std::cout << color[0] << std::endl;

			loc = glGetUniformLocation(shaderProgram, "mvp");

			if (loc != -1)
				glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mp));

			glUseProgram(0);
		}*/
		
		for (int i = 0; i < vertices.size(); i++)
		{
			verticeToBe.at(i) = _strdup(std::to_string(vertices.at(i)).c_str());
		}

		for (int i = 0; i < indices.size(); i++)
		{
			indiceToBe.at(i) = _strdup(std::to_string(indices.at(i)).c_str());
		}

		shaderController::texture(hasTextureActive);

		glUseProgram(shaderProgram);

		glBindVertexArray(vao);

		std::vector<float> fullData;

		int textureCounter = 0;

		width = size.x;
		height = size.y;

		right = position.x + (width);
		left = position.x - (width);
		top = position.y + (height);
		bottom = position.y - (height);

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

	int Sprite::isColliding(Sprite* &sprite1, Sprite* &sprite2)
	{
		int direction_colliding = 0;

		bool right_collide = sprite1->right > sprite2->left;

		if (right_collide)
		{
			std::cout << "right collide" << std::endl;
			direction_colliding = 1;
		}

		return direction_colliding;
	}

	void Sprite::update()
	{
		isSquare = false;
		isRect = false;
		isTri = false;
		isCube = false;
		isPoly = false;
		isUnOrdered = false;

		if (shape == 0)
		{
			isUnOrdered = true;
		}

		if (shape == 1)
		{
			isRect = true;
		}

		if (shape == 2)
		{
			isSquare = true;
		}

		if (shape == 3)
		{
			isTri = true;
		}

		if (shape == 4)
		{
			isCube = true;
		}

		indices.clear();
		vertices.clear();

		if (shape == 1 || shape == 0)
		{
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(0);

			vertices.push_back(1.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			vertices.push_back(-1.0f);
			vertices.push_back(0.0f);
			vertices.push_back(-1.0f);
			vertices.push_back(-1.0f);
			vertices.push_back(0.0f);
			vertices.push_back(-1.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);
		}

		else if (shape == 2)
		{
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(0);

			vertices.push_back(1.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.00f);
			vertices.push_back(1.0f);
			vertices.push_back(-1.0f);
			vertices.push_back(0.00f);
			vertices.push_back(-1.0f);
			vertices.push_back(-1.0f);
			vertices.push_back(0.00f);
			vertices.push_back(-1.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.00f);

			textures.push_back(1.0f);
			textures.push_back(1.0f);
			textures.push_back(1.0f);
			textures.push_back(0.0f);
			textures.push_back(0.0f);
			textures.push_back(0.0f);
			textures.push_back(0.0f);
			textures.push_back(1.0f);
		}

		else if (shape == 3)
		{
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(0);

			vertices.push_back(0.0f);
			vertices.push_back(0.5f);
			vertices.push_back(0.0f);
			vertices.push_back(-0.5f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.5f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}

		else if (shape == 4)
		{
			indices.push_back(0);
			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(0);
			indices.push_back(3);
			indices.push_back(1);
			
			indices.push_back(2);
			indices.push_back(6);
			indices.push_back(7);
			indices.push_back(2);
			indices.push_back(7);
			indices.push_back(3);
			
			indices.push_back(6);
			indices.push_back(4);
			indices.push_back(5);
			indices.push_back(6);
			indices.push_back(5);
			indices.push_back(7);
			
			indices.push_back(4);
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(4);
			indices.push_back(1);
			indices.push_back(5);
			
			indices.push_back(0);
			indices.push_back(4);
			indices.push_back(6);
			indices.push_back(0);
			indices.push_back(6);
			indices.push_back(2);
			
			indices.push_back(1);
			indices.push_back(5);
			indices.push_back(7);
			indices.push_back(1);
			indices.push_back(7);
			indices.push_back(3);
			
			vertices.push_back(-1.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);

			vertices.push_back(-1.0f);
			vertices.push_back(-1.0f);
			vertices.push_back(0.0f);

			vertices.push_back(1.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);

			vertices.push_back(1.0f);
			vertices.push_back(-1.0f);
			vertices.push_back(0.0f);
			
			vertices.push_back(-1.0f);
			vertices.push_back(1.0f);
			vertices.push_back(-1.0f);

			vertices.push_back(-1.0f);
			vertices.push_back(-1.0f);
			vertices.push_back(-1.0f);

			vertices.push_back(1.0f);
			vertices.push_back(1.0f);
			vertices.push_back(-1.0f);

			vertices.push_back(1.0f);
			vertices.push_back(-1.0f);
			vertices.push_back(-1.0f);
		}

		verticeToBe.resize(vertices.size());
		indiceToBe.resize(indices.size()); 

		for (int i = 0; i < vertices.size(); i++)
		{
			verticeToBe.at(i) = _strdup(std::to_string(vertices.at(i)).c_str());
		}

		for (int i = 0; i < indices.size(); i++)
		{
			indiceToBe.at(i) = _strdup(std::to_string(indices.at(i)).c_str());
		}
	}
}
