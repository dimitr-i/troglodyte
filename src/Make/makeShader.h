#pragma once

#include <string>

#include "../troglodyte.h"

namespace trog
{
	class makeShader
	{
	public:
		static std::string readShader(const char* filepath);

		static unsigned int shader(const char* shaderSource, unsigned int type);
	};

}