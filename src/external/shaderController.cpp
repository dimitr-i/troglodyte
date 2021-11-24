#include "shaderController.h"

void shaderController::uniform(int loc, int first)
{
	glUniform1i(loc, first);
}

void shaderController::uniform(int loc, int first, int second)
{
	glUniform2i(loc, first, second);
}

void shaderController::uniform(int loc, int first, int second, int third)
{
	glUniform3i(loc, first, second, third);
}

void shaderController::uniform(int loc, int first, int second, int third, int fourth)
{
	glUniform4i(loc, first, second, third, fourth);
}

void shaderController::uniform(int loc, unsigned int first)
{
	glUniform1i(loc, first);
}

void shaderController::uniform(int loc, unsigned int first, unsigned int second)
{
	glUniform2i(loc, first, second);
}

void shaderController::uniform(int loc, unsigned int first, unsigned int second, unsigned int third)
{
	glUniform3i(loc, first, second, third);
}

void shaderController::uniform(int loc, unsigned int first, unsigned int second, unsigned int third, unsigned int fourth)
{
	glUniform4i(loc, first, second, third, fourth);
}

void shaderController::uniform(int loc, float first)
{
	glUniform1f(loc, first);
}

void shaderController::uniform(int loc, float first, float second)
{
	glUniform2f(loc, first, second);
}

void shaderController::uniform(int loc, float first, float second, float third)
{
	glUniform3f(loc, first, second, third);
}

void shaderController::uniform(int loc, float first, float second, float third, float fourth)
{
	glUniform4f(loc, first, second, third, fourth);
}

int shaderController::getUniform(const char* loc, unsigned int program)
{
	if (program == 0)
		program = trog::shaderProgram;

	int uniformLoc = glGetUniformLocation(program, loc);

	if (uniformLoc != -1)
	{
		return uniformLoc;
	}
	else
	{
		std::cout << "Can't find uniform at location: " << loc << std::endl;
	}
}

void shaderController::texture(unsigned int ShowTextureOrNot, unsigned int program)
{
	if (program == 0)
		program = trog::shaderProgram;

	glUseProgram(program);
	unsigned int TextureBoolVertLoc = glGetUniformLocation(program, "showTextureFrag");
		
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, texture);

	if (TextureBoolVertLoc != -1)
	{
		uniform(TextureBoolVertLoc, ShowTextureOrNot);
		//std::cout << "GL UNFIROM" << std::endl;
	}
	else
		std::cout << "FONT RENDERER: FAILED TO FIND SHOW TEXTURE LOCATION" << std::endl;
	//glBindTexture(GL_TEXTURE_2D, 0);
			
	glUseProgram(0);
}