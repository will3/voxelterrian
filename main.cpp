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
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "Editor.h"
#include "Dispatcher.h"
#include "Terrian2.h"
#include "VoxelMaterial.h"
#include "EffectComposer.h"
#include "CopyPass.h"
#include "RenderPass.h"
#include "ShadowMap.h"

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

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	Dispatcher *dispatcher = new Dispatcher();
	Runner *runner = new Runner();
	float ratio = width / (float)height;
	Camera *camera = new PerspectiveCamera(60, ratio, 0.1f, 1000.0f);
	Scene *scene = new Scene();
	DirectionalLight *light = new DirectionalLight();
	scene->add(light);
	AmbientLight *ambientLight = new AmbientLight({ 0.5, 0.5, 0.5 });
	scene->add(ambientLight);

	VoxelMaterial *material = new VoxelMaterial();
	EditorCameraControl *camera_control;

	Terrian *terrian;
	int scenario = 1;
	if (scenario == 0) {
		camera_control = new EditorCameraControl();
		camera_control->camera = camera;
		runner->add(camera_control);

		terrian = new Terrian();
		terrian->light = light;
		terrian->scene = scene;
		terrian->material = material;
		terrian->dispatcher = dispatcher;
		terrian->camera_control = camera_control;
		terrian->set_draw_dis(3);
		runner->add(terrian);
	}
	else {
		Terrian2 *terrian2 = new Terrian2();
		runner->add(terrian2);
		terrian2->scene = scene;
		camera->position = glm::vec3(0, 200, -200);
		camera->target = glm::vec3(0, 0, 0);
	}
	
	__int64 last_tick = 0;

	Editor *editor = new Editor();

	EffectComposer *composer = new EffectComposer(renderer);
	RenderPass *renderPass = new RenderPass(scene, camera);
	composer->add_pass(renderPass);

	CopyPass *copyPass = new CopyPass();
	composer->add_pass(copyPass);
	copyPass->renderToScreen = true;

	ShadowMap *shadowMap = new ShadowMap(256, 256, 0.1, 1000);
	renderer->shadowMap = shadowMap;

	do {
		glfwPollEvents();

		ImGui_ImplGlfwGL3_NewFrame();

		if (scenario == 0) {
			editor->show(terrian, camera_control);
		}

		dispatcher->update();
		runner->update();

		composer->render();
		//copyPass->render(renderer, 0, shadowMap->renderTarget);

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		if (last_tick != 0) {
			runner->time_step = (get_time_stamp() - last_tick) / 1000.0f;
		}

		last_tick = get_time_stamp();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}

