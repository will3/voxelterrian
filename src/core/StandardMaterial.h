#pragma once

#include "Material.h"
#include <common/shader.hpp>

class StandardMaterial : public Material {
	void load() {
		programID = LoadShaders("shaders/standard.vertexshader", "shaders/standard.fragmentshader");
		MatrixID = glGetUniformLocation(programID, "MVP");
	}
};