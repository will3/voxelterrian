#pragma once

#include "Scene.h"
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include "RenderTarget.h"

class Renderer {
public:
	GLuint VertexArrayID;
	
	void render(Scene *scene, Camera *camera) {
		render(scene, camera, 0);
	}

	void render(Scene *scene, Camera *camera, RenderTarget *renderTarget) {
		if (renderTarget != 0) {
			if (!renderTarget->loaded) {
				renderTarget->load();
				renderTarget->loaded = true;
			}
		}

		if (renderTarget == 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->FramebufferName);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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