#include "Renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Renderer::render(Scene * scene, Camera * camera) {
	camera->update_view_matrix();
	for (Node *node : scene->nodes) {
		node->render(camera);
	}
}

Renderer::Renderer()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		throw std::exception("Failed to initialize GLFW\n");
	}

	int width = 1280;
	int height = 720;
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "kestrel", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		throw std::exception("Failed to open GLFW window.\n");
	}

	glfwMakeContextCurrent(window);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}
