#pragma once

#include <vector>
#include <GL\glew.h>

class Material;

class VoxelMesh {
private:
	GLuint vertexbuffer;
	GLuint colorbuffer;
	GLuint lightingbuffer;

public:
	Material * material;
	std::vector<GLint> vertices;
	std::vector<GLubyte> colors;
	std::vector<GLint> lighting;

	void load_buffer();

	void render();

	~VoxelMesh();
};