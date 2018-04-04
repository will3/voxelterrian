#include "DirectionalLight.h"
#include "Mask.h"

glm::vec3 unit_vector(glm::vec3 vector) {
	float dis = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	return vector / dis;
}

DirectionalLight::DirectionalLight() {
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setPosition(glm::vec3 position) {
	this->position = position;

	updateDirection();
}

