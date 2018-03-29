#include "Camera.h"

void Camera::update_view_matrix()
{
	View = glm::lookAt(position, target, up);
}
