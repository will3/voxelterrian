#pragma once

#include "Material.h"

class VoxelMaterial : public Material {
	void load() {
		programID = LoadShaders("shaders/voxel.vertexshader", "shaders/voxel.fragmentshader");
		MatrixID = glGetUniformLocation(programID, "MVP");
	}
};