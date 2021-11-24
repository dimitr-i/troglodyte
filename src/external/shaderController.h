#pragma once

#include "../troglodyte.h"

namespace shaderController
{
	int getUniform(const char* loc, unsigned int program = 0);
	void text(unsigned int ShowTextOrNo, unsigned int program = 0);
	void texture(unsigned int ShowTextureOrNo, unsigned int program = 0);

	void uniform(int loc, int first); // GLUNIFORM 1i
	void uniform(int loc, int first, int second); // GLUNIFORM 2i
	void uniform(int loc, int first, int second, int third); // GLUNIFORM 3i
	void uniform(int loc, int first, int second, int third, int fourth); // GLUNIFORM 4i

	void uniform(int loc, unsigned int first); // GLUNIFORM 1i
	void uniform(int loc, unsigned int first, unsigned int second); // GLUNIFORM 2i
	void uniform(int loc, unsigned int first, unsigned int second, unsigned int third); // GLUNIFORM 3i
	void uniform(int loc, unsigned int first, unsigned int second, unsigned int third, unsigned int fourth); // GLUNIFORM 4i

	void uniform(int loc, float first); // GLUNIFORM 1i
	void uniform(int loc, float first, float second); // GLUNIFORM 2i
	void uniform(int loc, float first, float second, float third); // GLUNIFORM 3i
	void uniform(int loc, float first, float second, float third, float fourth); // GLUNIFORM 4i
} 