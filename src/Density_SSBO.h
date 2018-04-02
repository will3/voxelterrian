#pragma once
#include <GL\glew.h>
#include <vector>

class Density_SSBO {
public:
	GLuint handle;
	std::vector<float> density;
	void load() {
		glGenBuffers(1, &handle);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, handle);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * density.size(), density.data(), GL_DYNAMIC_DRAW);
	}
};