#include "Renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShadowMap.h"

void Renderer::render(Scene * scene, Camera * camera, RenderTarget * renderTarget) {
	if (shadowMap != 0) {
		shadowMap->render(scene);
	}

	if (renderTarget == 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, window_width, window_height);
	}
	else {
		if (!renderTarget->loaded) {
			renderTarget->load();
			renderTarget->loaded = true;
		}
		renderTarget->bind();
		glViewport(0, 0, renderTarget->width, renderTarget->height);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (camera != 0) {
		camera->update_view_matrix();
	}

	for (Node *node : scene->nodes) {
		node->currentRenderer = this;
		node->update_matrix();
		node->render(camera);
	}
}

void Renderer::start_window(int width, int height) {
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		throw std::exception("Failed to initialize GLFW\n");
	}

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

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		throw std::exception("Failed to initialize GLEW\n");
	}

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// But on MacOS X with a retina screen it'll be 1024*2 and 768*2, so we get the actual framebuffer size:
	glfwGetFramebufferSize(window, &window_width, &window_height);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &VertexArrayID);
}
