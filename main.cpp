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
#include "BoxGeometry.h"
#include "StandardMaterial.h"
#include <glm/gtx/quaternion.hpp>
#include "SphereGeometry.h"
#include "CameraControl.h"

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
	Window *window = new Window();
	window->show(1280, 720);
	Renderer *renderer = new Renderer();
	renderer->window = window;
	renderer->load();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window->window, true);
	ImGui::StyleColorsDark();

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window->window, GLFW_STICKY_KEYS, GL_TRUE);

	float ratio = window->width / (float)window->height;
	Camera *camera = new PerspectiveCamera(60, ratio, 0.1f, 1000.0f);
	Scene *scene = new Scene();
	DirectionalLight *light = new DirectionalLight(0.9);
	light->setPosition(glm::vec3(100, 200, 50));

	scene->add(light);
	AmbientLight *ambientLight = new AmbientLight({ 0.5, 0.5, 0.5 });
	scene->add(ambientLight);

	camera->position = glm::vec3(0, 200, -200);
	camera->target = glm::vec3(0, 0, 0);

	EffectComposer *composer = new EffectComposer(renderer);

	RenderPass *renderPass = new RenderPass(scene, camera);
	composer->add_pass(renderPass);

	CopyPass *copyPass = new CopyPass();
	composer->add_pass(copyPass);
	copyPass->renderToScreen = true;

	//Material *material = new StandardMaterial();

	//SphereGeometry *geometry = new SphereGeometry(100.0, 16, 12);
	//Mesh *sphere = new Mesh(geometry, material);
	//scene->add(sphere);
	//sphere->position.x = -100.0;

	//BoxGeometry *boxGeometry = new BoxGeometry(10.0);
	//Mesh *box = new Mesh(boxGeometry, material);
	//scene->add(box);
	//box->position.x = 20.0;

	Runner *runner = new Runner();

	CameraControl *cameraControl = new CameraControl();
	runner->add(cameraControl);
	cameraControl->window = window;
	cameraControl->camera = camera;
	cameraControl->position = glm::vec3(0, 128, 0);
	cameraControl->pitch = - glm::pi<float>() / 4;

	ShadowMap *shadowMap = new ShadowMap(256, 256, 0.1, 1000, 1024, 1024);
	shadowMap->camera->position = light->position;
	shadowMap->camera->target = glm::vec3(0, 0, 0);
	scene->shadowMap = shadowMap;

	Terrian2 *terrian = new Terrian2();
	terrian->scene = scene;
	runner->add(terrian);

	Editor *editor = new Editor();
	editor->rock_color_gradient = terrian->rock_color_gradient;
	editor->load();

	do {
		glfwPollEvents();

		runner->update();

		shadowMap->render(renderer, scene);

		composer->render();

		ImGui_ImplGlfwGL3_NewFrame();
		editor->draw();

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window->window);

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window->window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window->window) == 0);

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	window->hide();

	return 0;
}

