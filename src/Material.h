#pragma once

#include <GL/glew.h>
#include "ReferenceCounted.h"

class Material : public ReferenceCounted {
public:
	GLuint programID;
	GLuint MatrixID;

	Material();

	~Material();
};