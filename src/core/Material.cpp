#include "Material.h"
#include <common/shader.hpp>

Material::Material() {
	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("shaders/voxel.vertexshader", "shaders/voxel.fragmentshader");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
}

Material::~Material() {
	glDeleteProgram(programID);
}
