#pragma once

class Material {
public:
	GLuint programID;
	GLuint MatrixID;

	Material() {
		// Create and compile our GLSL program from the shaders
		programID = LoadShaders("shaders/voxel.vertexshader", "shaders/voxel.fragmentshader");

		// Get a handle for our "MVP" uniform
		MatrixID = glGetUniformLocation(programID, "MVP");
	}

	~Material() {
		glDeleteProgram(programID);
	}
};