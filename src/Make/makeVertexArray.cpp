#include "makeVertexArray.h"

namespace trog
{
	unsigned int makeVertexArray::vao()
	{
		unsigned int TempVAO;

		glGenVertexArrays(1, &TempVAO);
		//glBindVertexArray(TempVAO);

		trog::getError();

		return TempVAO;
	}
}