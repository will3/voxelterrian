#pragma once
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Camera.h"
#include "Entity.h"
#include "Window.h"

class CameraControl : public Entity {
public:
	Window *window = 0;
	Camera *camera = 0;

	float horizontalAngle = 0;
	float verticalAngle = 0;
	float mouseSpeed = 0.001f;
	glm::vec3 position = glm::vec3(0, 0, 0);
	float speed = 100;

	void start() override {
		if (window == 0) throw std::exception("window cannot be empty");
		if (camera == 0) throw std::exception("camera cannot be empty");
	}

	void update() override {
		updateCamera();
	}

	void updateCamera() {
		if (!window->hasFocus()) {
			return;
		}
		float pi = glm::pi<float>();
		float delta = this->runner->delta;

		float width = window->width;
		float height = window->height;

		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window->window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window->window, width / 2, height / 2);

		// Compute new orientation
		horizontalAngle += mouseSpeed * float(width / 2 - xpos);
		verticalAngle += mouseSpeed * float(height / 2 - ypos);
		verticalAngle = fmin(pi / 2, fmax(-pi / 2, verticalAngle));

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		// Up vector
		glm::vec3 up = glm::cross(right, direction);

		// Move forward
		if (glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS) {
			position += direction * delta * speed;
		}
		// Move backward
		if (glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS) {
			position -= direction * delta * speed;
		}
		// Strafe right
		if (glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS) {
			position += right * delta * speed;
		}
		// Strafe left
		if (glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS) {
			position -= right * delta * speed;
		}

		camera->position = position;
		camera->target = position + direction;
	}
};