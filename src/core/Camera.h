#pragma once

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "types.h"

class Camera {
public:
	float fov;
	float ratio;
	float near;
	float far;

	Camera(float fov, float ratio, float near, float far);

	void update_projection_matrix();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection;

	// Camera matrix
	glm::mat4 View;

	glm::vec3 position = glm::vec3(0, 200, -200);
	glm::vec3 target = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);

	void update_view_matrix();
};