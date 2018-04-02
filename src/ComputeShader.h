#pragma once

#include <GL/glew.h>
#include "common/shader.hpp"

class ComputeShader {
	GLuint programID;

	ComputeShader() {
		LoadComputeShader("shaders/density_field_to_iso.compute");
	}

	~ComputeShader() {
		glDeleteProgram(programID);
	}
};