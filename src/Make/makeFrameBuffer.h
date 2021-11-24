#pragma once

#include "../troglodyte.h"

namespace trog
{
	class makeFrameBuffer
	{
	public:
		static unsigned int frameBuffer();
		static unsigned int texture();
		static unsigned int rbo();
		static void status();

		static void bind();
		static void unbind();
	};
}