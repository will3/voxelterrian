#include "Renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Renderer::render(Scene * scene, Camera * camera, RenderTarget * renderTarget) {
	if (!loaded) {
		load();
		loaded = true;
	}

	if (renderTarget == 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, window->width, window->height);
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

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &VertexArrayID);
}
