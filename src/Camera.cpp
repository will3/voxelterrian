#include "Camera.h"

Camera::Camera(float fov, float ratio, float near, float far) : fov(fov), ratio(ratio), near(near), far(far) {
	update_projection_matrix();
};

void Camera::update_projection_matrix() {
	Projection = glm::perspective(glm::radians(fov), ratio, near, far);
}

void Camera::update_view_matrix()
{
	View = glm::lookAt(position, target, up);
}
