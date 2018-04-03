#pragma once
#include "GL/glew.h"

class FrameBuffer {
	GLuint handle;
	void load() {
		glGenFramebuffers(1, &handle);
		glBindFramebuffer(GL_FRAMEBUFFER, handle);
	}
};