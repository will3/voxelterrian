#pragma once
#include "Shader.h"
#include "Material.h"

class ShaderMaterial : public Material {
	GLuint programID;
	Shader *shader;

public:

	void load() override {
		programID = ShaderLoader::LoadProgram(shader->programName);
		uniforms = shader->uniforms;
		uniforms.get_handles(programID);
	};

	void bind() override {
		glUseProgram(programID);

		glm::mat4 MVP = currentCamera->Projection * currentCamera->View * currentNode->matrix;
		uniforms.set("MVP", MVP);
	};

public:
	ShaderMaterial(Shader *shader) : shader(shader) {}
	~ShaderMaterial() {
		delete shader;
		glDeleteProgram(programID);
	}
};