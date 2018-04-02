#pragma once

#include "Material.h"
#include <common/shader.hpp>
#include "DirectionalLight.h"

class StandardMaterial : public Material {
public:
	GLuint light_dir;
	GLuint ambient_light;

	void load() {
		programID = LoadShaders("shaders/standard.vertexshader", "shaders/standard.fragmentshader");
		MatrixID = glGetUniformLocation(programID, "MVP");
		light_dir = glGetUniformLocation(programID, "light_dir");
		ambient_light = glGetUniformLocation(programID, "ambient_light");
	}

	void bind_uniforms(Node *node) {
		Scene *scene = node->scene;
		for (auto light : scene->directional_lights) {
			glUniform3f(light_dir, light->inverse_direction[0], light->inverse_direction[1], light->inverse_direction[2]);
			// only support one directional light
			break;
		}
		for (auto light : scene->ambient_lights) {
			glUniform3f(ambient_light, light->color[0], light->color[1], light->color[2]);
			// only support one ambient light
			break;
		}
	}
};