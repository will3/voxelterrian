#pragma once

#include "GL/glew.h"
#include "Node.h"

class DirectionalLight : public Node
{
public:
	glm::vec3 direction;
	glm::vec3 inverse_direction;
	glm::vec3 position;

	DirectionalLight();
	~DirectionalLight();

	void setPosition(glm::vec3 position);

	void updateDirection();
};

