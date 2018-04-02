#pragma once

#include "Scene.h"
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW\glfw3.h>

class Renderer {
public:
	GLuint VertexArrayID;
	void render(Scene *scene, Camera *camera);
	void start_window(int width, int height);
	GLFWwindow * window;
	int window_width;
	int window_height;
	Renderer();
	~Renderer();
};