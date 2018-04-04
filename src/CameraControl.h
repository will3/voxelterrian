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

	float yaw = 0;
	float pitch = 0;
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
		glfwSetCursorPos(window->window, width / 2, height / 2);

		float diffX = width / 2 - xpos;
		float diffY = height / 2 - ypos;

		// Compute new orientation
		yaw -= mouseSpeed * diffX;
		pitch += mouseSpeed * diffY;

		pitch = fmin(pi / 2, fmax(-pi / 2, pitch));

		glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
		glm::quat qRoll = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));
		glm::quat quat = qRoll * qPitch * qYaw;

		glm::vec3 direction = glm::vec3(0, 0, 1) * quat;

		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = glm::cross(direction, up);
		glm::vec3 forward = glm::cross(up, right);

		if (glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS) {
			position += forward * delta * speed;
		}
		if (glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS) {
			position -= forward * delta * speed;
		}
		if (glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS) {
			position += right * delta * speed;
		}
		if (glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS) {
			position -= right * delta * speed;
		}
		if (glfwGetKey(window->window, GLFW_KEY_R) == GLFW_PRESS) {
			position += up * delta * speed;
		}
		if (glfwGetKey(window->window, GLFW_KEY_F) == GLFW_PRESS) {
			position -= up * delta * speed;
		}

		camera->position = position;
		camera->target = position + direction;
	}
};