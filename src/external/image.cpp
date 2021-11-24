#include "Image.h"

#include "../Renderer/camera.h"
#include "shaderController.h"

#include "glm/gtx/string_cast.hpp"

namespace trog
{
	unsigned int Image::playButtonTexture = 0;
	unsigned int Image::spriteImageTexture = 0;
	unsigned int Image::playerTexture = 0;
	unsigned int Image::heartTexture = 0;

	void Image::setImageValues()
	{
		Image::playButtonTexture = makeTexture::texture("../troglodyte/res/Images/playButton.png");
		Image::spriteImageTexture = makeTexture::texture("../troglodyte/res/Images/spritePlaceHolder.png");
		Image::playerTexture = makeTexture::texture("../troglodyte/res/Images/mainPlayer.png");
		Image::heartTexture = makeTexture::texture("../troglodyte/res/Images/heart.png");
	}

	void Image::renderImageFromFilepath(const char* filepath, glm::vec3 position, glm::vec3 size, glm::vec3 rotate, bool gui)
	{
		unsigned int image = makeTexture::texture(filepath);

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

			if (!gui)
				projection = camera::getProjection();

			if (!gui)
				view = camera::getView();

			model = glm::translate(model, position);

			model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));

			model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z));

			model = glm::scale(model, size);

			glm::mat4 mvp = projection * glm::mat4(glm::mat3(view)) * model;

			glUseProgram(shaderProgram);

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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		getError("UPDATE SPRITE");
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, image);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);

		getError();
	}

	void Image::renderImageFromUI(unsigned int image, glm::vec3 position, glm::vec3 size, glm::vec3 rotate, bool gui)
	{
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

			if (!gui)
				projection = camera::getProjection();

			if (!gui)
				view = camera::getView();

			model = glm::translate(model, position);

			model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));

			model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z));

			model = glm::scale(model, size);

			glm::mat4 mvp = projection * glm::mat4(glm::mat3(view)) * model;

			glUseProgram(shaderProgram);

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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		getError("UPDATE SPRITE");
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, image);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);

		getError();
	}
}
