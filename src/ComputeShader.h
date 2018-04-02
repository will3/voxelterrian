#pragma once

#include <GL/glew.h>
#include "common/shader.hpp"
#include <vector>

class ComputeShader {
public:
	GLuint programID;

	ComputeShader() {
		programID = LoadComputeShader("shaders/density_field_to_iso.compute");
	}

	void bind() {
		glUseProgram(programID);
	}

	void dispatch(GLuint ssbo) {
		glDispatchCompute(1, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

		GLfloat *ptr;
		ptr = (GLfloat *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

		std::vector<GLfloat> results;

		for (int i = 0; i < 1; i++) {
			results.push_back(ptr[i]);
		}

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	~ComputeShader() {
		glDeleteProgram(programID);
	}
};