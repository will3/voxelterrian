#pragma once

#include <vector>
#include <GL\glew.h>
#include "Node.h"

class Material;

class Mesh : public Node {
private:
	GLuint VertexArrayID;
	GLuint vertexbuffer;
	GLuint colorbuffer;
	GLuint lightingbuffer;
	GLuint elementbuffer;

public:
	Material * material;
	std::vector<GLint> vertices;
	std::vector<GLubyte> colors;
	std::vector<GLint> lighting;
	std::vector<unsigned int> indices;

	void load_buffer();

	void render(Camera *camera);

	int num_vertices();

	~Mesh();
};