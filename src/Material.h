#pragma once

#include <GL/glew.h>

class Material {
public:
	GLuint programID;
	GLuint MatrixID;

	Material();

	~Material();
};