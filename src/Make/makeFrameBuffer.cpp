#include "makeFrameBuffer.h"

namespace trog
{
	unsigned int makeFrameBuffer::frameBuffer()
	{
		unsigned int frameBufferTemp;
		glGenFramebuffers(1, &frameBufferTemp);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferTemp);

		return frameBufferTemp;
	}

	unsigned int makeFrameBuffer::texture()
	{
        unsigned int textureColorbuffer;
        glGenTextures(1, &textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

		return textureColorbuffer;
	}

	unsigned int makeFrameBuffer::rbo()
	{
        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer is not complete." << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return rbo;
	}

    void makeFrameBuffer::status()
    {
        GLenum status;
        status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        switch (status) {
        case GL_FRAMEBUFFER_COMPLETE:
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            /* choose different formats */
            break;

        default:
            /* programming error; will fail on all hardware */
            fputs("Framebuffer Error\n", stderr);
            exit(-1);
        }
    }

    void makeFrameBuffer::bind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glClearColor(0.50f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void makeFrameBuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, fbo_text);
    }
}