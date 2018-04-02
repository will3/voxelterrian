#pragma once

#include "Scene.h"
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include "RenderTarget.h"

class Renderer {
public:
	GLuint VertexArrayID;
	RenderTarget *currentRenderTarget;
	
	void render(Scene *scene, Camera *camera) {
		render(scene, camera, 0);
	}

	void render(Scene *scene, Camera *camera, RenderTarget *renderTarget) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (renderTarget != 0) {
			currentRenderTarget = renderTarget;
			glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->FramebufferName);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		if (camera != 0) {
			camera->update_view_matrix();
		}

		for (Node *node : scene->nodes) {
			node->update_matrix();
			node->render(camera);
		}
	}

	void start_window(int width, int height);
	GLFWwindow * window;
	int window_width;
	int window_height;
	Renderer();
	~Renderer();
};