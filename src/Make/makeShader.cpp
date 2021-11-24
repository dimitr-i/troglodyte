#include "makeShader.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

namespace trog
{
	std::string makeShader::readShader(const char* filepath)
	{
		std::fstream file(filepath);

		std::string temp;

		std::string line;
		std::string shaderSourceTemp;

		std::vector <std::string> vec;

		if (std::getline(file, temp))
		{
			vec.push_back(temp);
			while (std::getline(file, line))
			{
				if (line.empty())
					continue;

				//std::cout << line << std::endl;

				vec.push_back(line);
			}
		}
		else
			std::cout << "SHADER MAKING: INVALID FILE PATH." << std::endl;

		for (int i = 0; i < vec.size(); i++)
		{
			//std::cout << vec.at(i) << std::endl;
			shaderSourceTemp = shaderSourceTemp + vec[i] + "\n";
		}

		return shaderSourceTemp;
	}

	unsigned int makeShader::shader(const char* shaderSource, unsigned int type)
	{
		if (type == 0)
		{
			unsigned int tempVertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(tempVertex, 1, &shaderSource, NULL);
			glCompileShader(tempVertex);

			trog::getError();

			GLint Result = GL_FALSE;
			int InfoLogLength;

			// Check Vertex Shader
			glGetShaderiv(tempVertex, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(tempVertex, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 0) 
			{
				std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
				glGetShaderInfoLog(tempVertex, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
				printf("%s\n", &VertexShaderErrorMessage[0]);
			}

			trog::getError();

			return tempVertex;
		}
		else if (type == 1)
		{
			unsigned int tempFragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(tempFragment, 1, &shaderSource, NULL);
			glCompileShader(tempFragment);

			trog::getError();

			GLint Result = GL_FALSE;
			int InfoLogLength;

			// Check Vertex Shader
			glGetShaderiv(tempFragment, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(tempFragment, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 0)
			{
				std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
				glGetShaderInfoLog(tempFragment, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
				printf("%s\n", &VertexShaderErrorMessage[0]);
			}

			trog::getError();

			return tempFragment;
		}
		else
			;
	}
}
