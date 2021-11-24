#pragma once

#include "../troglodyte.h"

namespace trog
{
	class makeTexture
	{
	public:
		static unsigned int texture(const char* filepath);
		static unsigned int s_Texture(std::vector<std::string> faces);
	};

}
