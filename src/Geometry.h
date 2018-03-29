#pragma once
#include "ReferenceCounted.h"
#include <GL\glew.h>
#include <vector>

class Geometry : public ReferenceCounted
{
private:
	GLuint vertexbuffer;
	GLuint colorbuffer;
	GLuint lightingbuffer;
	GLuint elementbuffer;

public:
	bool vbo_loaded = false;

	std::vector<GLint> vertices;
	std::vector<GLint> colors;
	std::vector<GLint> lighting;
	std::vector<unsigned int> indices;

	void load_vbo();
	int num_vertices();
	void bind_vbo();
	void unbind_vbo();

	Geometry();
	~Geometry();
};

