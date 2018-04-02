#pragma once

#include <GL/glew.h>
#include <common/shader.hpp>

class Material {
public:
	GLuint programID;

	bool loaded = false;

	Node *current_node;

	Camera *current_camera;

	virtual void load() {};

	virtual void bind_uniforms() {};

	Material() {};

	~Material() {
		glDeleteProgram(programID);
	}
};