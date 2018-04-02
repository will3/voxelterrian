#pragma once
#include <GL\glew.h>
#include <vector>

class Density_SSBO {
public:
	std::vector<float> density;
	void load() {
		GLuint ssbo;
		glGenBuffers(1, &ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * density.size(), density.data(), GL_STATIC_READ);
	}
};