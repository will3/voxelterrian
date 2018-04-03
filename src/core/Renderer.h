#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "Scene.h"
#include "Camera.h"
#include "RenderTarget.h"

class Renderer {
public:
	GLuint VertexArrayID;
	GLFWwindow * window;
	int window_width;
	int window_height;
	ShadowMap *shadowMap = 0;

	Renderer()
	{
	}

	void render(Scene *scene, Camera *camera) {
		render(scene, camera, 0);
	}

	void render(Scene *scene, Camera *camera, RenderTarget *renderTarget);

	void start_window(int width, int height);

	~Renderer();
};