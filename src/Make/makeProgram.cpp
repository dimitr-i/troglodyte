#include "makeProgram.h"

#include <iostream>

namespace trog
{
	unsigned int makeProgram::program(unsigned int vertexShader, unsigned int fragmentShader)
	{
		trog::getError("BEFORE PROGRAM MADE");

		trog::getError();

		unsigned int tempProgram = glCreateProgram();

		trog::getError("make program");

		glAttachShader(tempProgram, vertexShader);
		glAttachShader(tempProgram, fragmentShader);

		trog::getError("SHADER ATTACH");

		glLinkProgram(tempProgram);
		glValidateProgram(tempProgram);

		trog::getError("greg");

		GLint success;
		GLchar infoLog[1024];

		glGetProgramiv(tempProgram, GL_LINK_STATUS, &success);
		getError("GET IV!");
		if (!success)
		{
			glGetProgramInfoLog(tempProgram, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
			getError("grental");
		}

		//glUseProgram(tempProgram);

		getError("useProgram");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		getError("Del shaders");

		//glUseProgram(0);

		return tempProgram;
	}
}
