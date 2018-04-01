#pragma once
#include <GL\glew.h>
#include <vector>

class Geometry
{
private:
	GLuint vertexbuffer = 0;
	GLuint colorbuffer = 0;
	GLuint lightingbuffer = 0;
	GLuint elementbuffer = 0;
	void unload_vbo();

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

