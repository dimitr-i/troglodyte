#include "rect.h"
#include "external/shaderController.h"

namespace trog
{
	rect::rect(float _width, float _height)
	{
		width = _width;
		height = _height;
	}

	void rect::renderRect(glm::vec3 position, glm::vec3 size, glm::vec3 rotate, glm::vec3 color, bool fromCenter)
	{
		shaderController::texture(0);

		position.x = position.x / SCR_WIDTH - 1.0f;
		position.y = position.y / SCR_HEIGHT;

		size.x = size.x / SCR_WIDTH;
		size.y = size.y / SCR_HEIGHT;

		std::vector<float> vertices;
		std::vector<float> textures;
		std::vector<unsigned int> indices;

		if (true)
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

		//std::cout << shape << std::endl;
		if (is3d)
		{
			glm::mat4 projection = glm::mat4(1.0f);
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);

			model = glm::translate(model, position);

			model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));

			model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z));

			model = glm::scale(model, size);

			glm::mat4 mvp = projection * glm::mat4(glm::mat3(view)) * model;

			glUseProgram(shaderProgram);

			int loc = shaderController::getUniform("u_Color");

			if (loc != -1)
			{
				shaderController::uniform(loc, color.x, color.y, color.z);
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

		getError("UPDATE SPRITE");

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);

		getError();
	}

	void rect::renderRectNS(float _x, float _y, glm::vec3 color)
	{
		glm::vec3 position = glm::vec3(_x, _y, 1.0f);
		glm::vec3 size = glm::vec3(width, height, 1.0f);

		shaderController::texture(0);

		position.x = position.x;
		position.y = position.y;

		size.x = size.x;
		size.y = size.y;

		std::vector<float> vertices;
		std::vector<float> textures;
		std::vector<unsigned int> indices;

		if (true)
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

		//std::cout << shape << std::endl;
		if (is3d)
		{
			glm::mat4 projection = glm::ortho(0.0f, SCR_WIDTH, 0.0f, SCR_HEIGHT, -1.0f, 1.0f);
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, position);
			model = glm::scale(model, size);

			glm::mat4 mvp = projection * model;

			glUseProgram(shaderProgram);

			int loc = shaderController::getUniform("u_Color");

			if (loc != -1)
			{
				shaderController::uniform(loc, color.x, color.y, color.z);
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

		getError("UPDATE SPRITE");

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);

		getError();
	}
}