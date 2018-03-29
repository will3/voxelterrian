#pragma once

#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Chunks.h"
#include "DirectionalLight.h"
#include "Mesher.h"
#include "Runner.h"
#include "Terrian.h"
#include "EditorCameraControl.h"
#include "common\shader.hpp"
#include "Mesh.h"
#include "Material.h"
#include "Scene.h"
#include "Renderer.h"
#include "Test.h"

using namespace glm;
using namespace std::chrono;

// helper macros for reacting on unexpected states
#define _TOSTR(x) #x
#define TOSTR(x) _TOSTR(x)
#define FASSERT(expr) if (!(expr)) { throw IrrIMGUI::Debug::ExAssert(__FILE__ "[" TOSTR(__LINE__) "] Assertion failed: \'" TOSTR(expr) "'\n"); }

__int64 get_time_stamp() {
	return duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()).count();
}

int main() {
	Renderer *renderer = new Renderer();
	int width = 1280;
	int height = 720;
	renderer->start_window(width, height);
	GLFWwindow *window = renderer->window;

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	Runner *runner = new Runner();
	DirectionalLight *light = new DirectionalLight();

	float ratio = width / (float)height;
	Camera *camera = new Camera(60, ratio, 0.1f, 1000.0f);

	Scene *scene = new Scene();

	Material *material = new Material();

	Terrian *terrian = new Terrian();
	terrian->light = light;
	terrian->scene = scene;
	terrian->material = material;
	terrian->set_draw_dis(6);
	runner->add(terrian);

	EditorCameraControl *cameraControl = new EditorCameraControl();
	cameraControl->camera = camera;
	cameraControl->terrian = terrian;
	runner->add(cameraControl);

	//Test *test = new Test();
	//test->scene = scene;
	//test->get_mesh()->position = { 5.0, 0.0, 0.0 };
	//runner->add(test);

	//Test *test2 = new Test();
	//test2->scene = scene;
	//runner->add(test2);

	__int64 last_tick = 0;

	do {
		runner->update();

		renderer->render(scene, camera);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (last_tick != 0) {
			runner->time_step = (get_time_stamp() - last_tick) / 1000.0f;
		}

		last_tick = get_time_stamp();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

