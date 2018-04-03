#pragma once

#include "Material.h"
#include <common/shader.hpp>
#include "DirectionalLight.h"
#include <map>
#include "Shader.h"

class StandardMaterial : public Material {
public:
	GLuint programID;
	UniformAttributes uniforms;

	StandardMaterial() {
		uniforms.add(UniformAttribute("light_dir"));
		uniforms.add(UniformAttribute("ambient_light"));
		uniforms.add(UniformAttribute("shadowMap"));
		uniforms.add(UniformAttribute("shadowMVP"));
	}

	void load() {
		programID = LoadShaders("shaders/standard.vertexshader", "shaders/standard.fragmentshader");
		uniforms.get_handles(programID);
	}

	void bind() override {
		glUseProgram(programID);

		for (auto light : scene->directional_lights) {
			uniforms.set("light_dir", light->inverse_direction[0], light->inverse_direction[1], light->inverse_direction[2]);
			// only support one directional light
			break;
		}
		for (auto light : scene->ambient_lights) {
			uniforms.set("ambient_light", light->color[0], light->color[1], light->color[2]);
			// only support one ambient light
			break;
		}

			/*uniforms.set_texture("shadowMap", currentRenderer->shadowMap->renderTarget->depthTexture);
			Camera *camera = currentRenderer->shadowMap->camera;
			glm::mat4 MVP = camera->Projection * camera->View;
			uniforms.set("shadowMVP", MVP);*/
	}
};