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

void set_up_game(Runner *runner) {
	DirectionalLight *light = new DirectionalLight();

	Terrian *terrian = new Terrian();
	terrian->light = light;
	//terrian->scene = scene;
	//terrian->voxelMaterial = voxelMaterial;
	runner->add(terrian);

	EditorCameraControl *cameraControl = new EditorCameraControl();
	//cameraControl->camera = camera;
	cameraControl->terrian = terrian;
	runner->add(cameraControl);
}

int main() {
	Renderer *renderer = new Renderer();
	GLFWwindow *window = renderer->window;

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	Runner *runner = new Runner();
	set_up_game(runner);

	Material *material = new Material();

	Mesh *mesh = new Mesh();
	mesh->material = material;
	mesh->vertices.insert(mesh->vertices.end(), {
		-1, -1, 0,
		1, -1, 0,
		0,  1, 0
	});
	mesh->colors.insert(mesh->colors.end(), {
		255,0,0,
		0,255,0,
		0,0,255
	});
	mesh->lighting.insert(mesh->lighting.end(), {
		15, 15, 15
	});
	mesh->indices.insert(mesh->indices.end(), {
		0,1,2
		});
	mesh->load_buffer();
	Camera *camera = new Camera();

	Scene *scene = new Scene();
	scene->add(mesh);

	__int64 last_tick = 0;

	do {
		runner->update();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

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

	// Cleanup VBO and shader
	delete mesh;
	delete material;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

