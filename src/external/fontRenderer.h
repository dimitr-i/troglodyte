#pragma once

//#include "all_includes.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <map>
#include <string>

#include "../troglodyte.h"

namespace trog
{
	class fontRenderer
	{
	public:
		static void initText(const char* font);
		static void renderText(std::string text, float x, float y, float scale, glm::vec3 color, bool centerW = false, bool centerH = false);

		static float getWidthOfText(std::string text, float scale);
		static float getHeightOfText(std::string text, float scale);
	};
}
