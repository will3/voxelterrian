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
	if (!geometry->loaded) {
		geometry->load();
		geometry->loaded = true;
	}

	if (!material->loaded) {
		material->load();
		material->loaded = true;
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

	material->bind_uniforms(this);

	geometry->bind();

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		geometry->get_indices_count(),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);

	geometry->unbind();
}

Mesh::~Mesh() {
}
