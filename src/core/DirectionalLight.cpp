#include "DirectionalLight.h"
#include "Raycast.h"
#include "Mask.h"

glm::vec3 unit_vector(glm::vec3 vector) {
	float dis = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	return vector / dis;
}
DirectionalLight::DirectionalLight() {
	direction = unit_vector({ 0.5f, 1.0f, -0.1f });
	inverse_direction = direction * -1.0f;
}

DirectionalLight::~DirectionalLight()
{
}