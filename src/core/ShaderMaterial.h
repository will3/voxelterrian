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
	};

	void bind() override {
		glUseProgram(programID);
		uniforms.get_handles(programID);
	};

public:
	ShaderMaterial(Shader *shader) : shader(shader) {}
	~ShaderMaterial() {
		delete shader;
		glDeleteProgram(programID);
	}
};