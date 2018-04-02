#pragma once

#include "GL/glew.h"
#include "Node.h"

class DirectionalLight : public Node
{
public:
	glm::vec3 direction;
	glm::vec3 inverse_direction;
	DirectionalLight();
	~DirectionalLight();
};

