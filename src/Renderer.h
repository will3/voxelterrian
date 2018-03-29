#pragma once

#include "Scene.h"
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW\glfw3.h>

class Renderer {
public:
	void render(Scene *scene, Camera *camera);
	GLFWwindow * window;
	Renderer();
};