#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "Scene.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "Window.h"

class Renderer {
public:
	GLuint VertexArrayID;
	bool loaded = false;
	Window *window;

	Renderer()
	{
	}

	void render(Scene *scene, Camera *camera) {
		render(scene, camera, 0);
	}

	void render(Scene *scene, Camera *camera, RenderTarget *renderTarget);

	void load() {
		// Initialize GLEW
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			getchar();
			glfwTerminate();
			throw std::exception("Failed to initialize GLEW\n");
		}

		// Dark blue background
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		// Cull triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
	}

	~Renderer();
};