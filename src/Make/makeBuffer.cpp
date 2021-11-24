#include "makeBuffer.h"

namespace trog
{
	unsigned int makeBuffer::vbo()
	{
		unsigned int TempVBO;
		glGenBuffers(1, &TempVBO);
		//glBindBuffer(GL_ARRAY_BUFFER, TempVBO);

		trog::getError();

		return TempVBO;
	}

	unsigned int makeBuffer::ebo()
	{
		unsigned int TempEBO;
		glGenBuffers(1, &TempEBO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TempEBO);

		trog::getError();

		return TempEBO;
	}
}
