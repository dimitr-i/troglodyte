#pragma once

#include "../troglodyte.h"
#include "../Make/makeTexture.h"

namespace trog
{
	class Image
	{
	public:
		static unsigned int playButtonTexture;
		static unsigned int spriteImageTexture;
		static unsigned int playerTexture;
		static unsigned int heartTexture;

		static void setImageValues();
		static void renderImageFromFilepath(const char* filepath, glm::vec3 position, glm::vec3 size, glm::vec3 rotate, bool gui);
		static void renderImageFromUI(unsigned int image, glm::vec3 position, glm::vec3 size, glm::vec3 rotate, bool gui);
	};
}