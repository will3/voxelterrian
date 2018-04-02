#pragma once

#include <GL/glew.h>
#include <common/shader.hpp>
#include "Shader.h"

class Scene;

class Material {
public:
	bool loaded = false;

	UniformAttributes uniforms;
	Scene *scene;

	virtual void load() {};

	virtual void bind() {};

	Material() {};

	~Material() {};
};