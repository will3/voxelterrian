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
	terrian->set_draw_dis(3);
	runner->add(terrian);
	EditorCameraControl *camera_control = new EditorCameraControl();
	camera_control->camera = camera;
	camera_control->terrian = terrian;
	runner->add(camera_control);

	__int64 last_tick = 0;

	do {
		glfwPollEvents();

		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("Editor");
		static int camera_speed = 1;
		ImGui::Text("Generation Speed");
		ImGui::RadioButton("1", &camera_speed, 1); ImGui::SameLine();
		ImGui::RadioButton("2", &camera_speed, 2); ImGui::SameLine();
		ImGui::RadioButton("4", &camera_speed, 4); ImGui::SameLine();
		ImGui::RadioButton("8", &camera_speed, 8); ImGui::SameLine();
		ImGui::RadioButton("16", &camera_speed, 16);
		ImGui::End();

		camera_control->speed = camera_speed * 4;
		//ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		//ImGui::ShowDemoWindow(&show_demo_window);

		runner->update();

		renderer->render(scene, camera);

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

