#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"

Mesh::Mesh(Geometry * geometry, Material * material) : geometry(geometry), material(material)
{
}

Geometry * Mesh::get_geometry()
{
	return geometry;
}

Material * Mesh::get_material()
{
	return material;
}

void Mesh::render(Camera * camera) {
	if (!geometry->vbo_loaded) {
		geometry->load_vbo();
		geometry->vbo_loaded = true;
	}

	// Use our shader
	glUseProgram(material->programID);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = matrix;

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = camera->Projection * camera->View * Model; // Remember, matrix multiplication is the other way around

											   // Send our transformation to the currently bound shader, 
											   // in the "MVP" uniform
	glUniformMatrix4fv(material->MatrixID, 1, GL_FALSE, &MVP[0][0]);

	geometry->bind_vbo();

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		geometry->indices.size(),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);

	geometry->unbind_vbo();
}

Mesh::~Mesh() {
}
