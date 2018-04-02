#pragma once

#include "Material.h"
#include "Node.h"

class ShadowMaterial : public Material {
public:
	GLuint depthMatrixID;
	glm::mat4 depthMVP;

	void load() override {
		programID = LoadShaders("shaders/standard.vertexshader", "shaders/standard.fragmentshader");
	};

	void bind_uniforms() override {
		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
	};
};