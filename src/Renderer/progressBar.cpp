#include "ProgressBar.h"
#include "../external/fontRenderer.h"
#include "../external/shaderController.h"
#include "../Renderer/camera.h"

namespace trog
{
	void ProgressBar::bar(float percentage)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(25.0f, 50.0f, 1.0f));

		glUseProgram(shaderProgram);

		glm::mat4 mvp = camera::getProjection() * camera::getView() * model;

		int loc = glGetUniformLocation(shaderProgram, "u_Color");

		if (loc != -1)
			glUniform3f(loc, 0.25f, 0.25f, 0.25f);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

		float vertices[] = {
			1.5f,  0.125f, 0.0f,
			0.5f, -0.125f, 0.0f,
			-0.5f, -0.125f, 0.0f,
			-0.5f,  0.125f, 0.0f
		};

		unsigned int indices[] = {
			0, 2, 1,
			3, 2, 0,
		};

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);
	}
}