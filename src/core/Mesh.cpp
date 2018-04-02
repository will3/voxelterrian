#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"
#include "Scene.h"

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

	if (!scene->override_material) {
		glUseProgram(material->programID);

		material->current_node = this;
		material->current_camera = camera;
		material->bind_uniforms();
	}

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
