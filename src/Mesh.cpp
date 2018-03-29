#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"

void Mesh::load_buffer() {
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLint), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLubyte), colors.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &lightingbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lightingbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLint), lighting.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void Mesh::render(Camera * camera) {
	// Use our shader
	glUseProgram(material->programID);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = camera->Projection * camera->View * Model; // Remember, matrix multiplication is the other way around

											   // Send our transformation to the currently bound shader, 
											   // in the "MVP" uniform
	glUniformMatrix4fv(material->MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribIPointer(0, 3, GL_INT, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribIPointer(1, 3, GL_UNSIGNED_BYTE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, lightingbuffer);
	glVertexAttribIPointer(2, 1, GL_INT, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

int Mesh::num_vertices()
{
	return vertices.size() / 3;
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteBuffers(1, &lightingbuffer);
	glDeleteBuffers(1, &elementbuffer);

	glDeleteVertexArrays(1, &VertexArrayID);
}
