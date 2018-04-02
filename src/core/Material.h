#pragma once

#include <GL/glew.h>
#include <common/shader.hpp>

class Material {
public:
	GLuint programID;
	GLuint MatrixID;

	bool loaded = false;

	virtual void load() {};

	virtual void bind_uniforms(Node *node) {};

	Material() {};

	~Material() {
		glDeleteProgram(programID);
	}
};