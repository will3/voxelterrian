#pragma once;
#include "Geometry.h"

class VoxelGeometry : public Geometry {
private:
	GLuint vertexbuffer = 0;
	GLuint colorbuffer = 0;
	GLuint lightingbuffer = 0;
	GLuint elementbuffer = 0;
public:
	std::vector<GLint> vertices;
	std::vector<GLint> colors;
	std::vector<GLint> lighting;

	void unload_vbo() {
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &colorbuffer);
		glDeleteBuffers(1, &lightingbuffer);
		glDeleteBuffers(1, &elementbuffer);
	}

	void load_vbo() {
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLint), vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLint), colors.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &lightingbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, lightingbuffer);
		glBufferData(GL_ARRAY_BUFFER, lighting.size() * sizeof(GLint), lighting.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		loaded = true;
	}

	void bind_vbo()
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribIPointer(0, 3, GL_INT, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribIPointer(1, 3, GL_INT, 0, (void*)0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, lightingbuffer);
		glVertexAttribIPointer(2, 1, GL_INT, 0, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	}

	void unbind_vbo()
	{
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
};