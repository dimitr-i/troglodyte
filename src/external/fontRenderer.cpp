#include "fontRenderer.h"
#include "shaderController.h"

struct Character
{
	unsigned int textureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

std::map<char, Character> Characters;

namespace trog
{
	void fontRenderer::initText(const char* font)
	{
		std::cout << "FONT RENDERER INIT" << std::endl;

		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			std::cout << "ERROR FREETYPE: COULD NOT INITIAILIZE FREETYPE LIBRARY" << std::endl;

		glUseProgram(textProgram);
		unsigned int textLoc = glGetUniformLocation(textProgram, "Textprojection");
		if (textLoc != -1)
			glUniformMatrix4fv(textLoc, 1, GL_FALSE, glm::value_ptr(textProjection));
		else
			std::cout << "FEW" << std::endl;
		glUseProgram(0);

		FT_Face face;
		if (FT_New_Face(ft, font, 0, &face))
			std::cout << "ERROR FREETYPE: COULD NOT LOAD FONT" << std::endl;
		
		else
		{
			FT_Set_Pixel_Sizes(face, 0, 48);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

			for (unsigned char c = 0; c < 128; c++)
			{
				// load character glyph 
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
					continue;
				}
				// generate texture
				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
				// set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// now store character for later use
				Character character = {
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x
				};
				Characters.insert(std::pair<char, Character>(c, character));
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		getError();
	}

	void fontRenderer::renderText(std::string text, float x, float y, float scale, glm::vec3 color, bool centerW, bool centerH)
	{
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		getError();

		//std::cout << "rendering text: " << text << std::endl;

		getError();

		glUseProgram(textProgram);
		unsigned int textColorLoc = glGetUniformLocation(textProgram, "u_TextColor");
		if (textColorLoc != -1)
			glUniform3f(textColorLoc, color.x, color.y, color.z);
		else
			std::cout << "NO COLOR!" << std::endl;
		//glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(textVao);

		float width = 0.0f;
		float height = 0.0f;

		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			if (centerW)
			{
				xpos -= getWidthOfText(text, scale) / 2.0f; // NOT CENTERED? CHECK WHY AND HOW TO FIX?
			}

			if (centerH)
			{
				ypos -= getHeightOfText(text, scale) / 2.0f;
			}

			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.textureID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, textVbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			getError("draw arays");
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
		}

		//std::cout << width << std::endl;

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		getError();

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	}

	float fontRenderer::getWidthOfText(std::string text, float scale)
	{
		float width = 0.0f;

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			float w = ch.Size.x * scale;

			width += w;
			// update VBO for each character
		}

		//std::cout << width << std::endl;

		return width;
	}

	float fontRenderer::getHeightOfText(std::string text, float scale)
	{
		float height = 0.0f;
		float padding = 2.0f;

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			float h = ch.Size.y * scale;

			if (h > height)
			{
				height = h;
			}
			// update VBO for each character
		}

		return height + padding;
	}
}
