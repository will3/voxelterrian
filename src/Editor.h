#pragma once

#include "Noise.h"
#include "imgui.h"
#include <math.h>
#include "imgui_color_gradient.h"
#include <json.hpp>
#include <iostream>
#include <fstream>  
#include "Terrian2.h"

ImGradientMark *draggingMark;
ImGradientMark *selectedMark;

using json = nlohmann::json;

class Editor {
public:
	ImGradient *rock_color_gradient;
	DirectionalLight *directionalLight;
	std::string fileName = "editor";
	Terrian2 *terrian;

	void draw() {
		bool terrianChanged = false;
		bool changed = false;
		if (ImGui::TreeNode("lighting")) {
			changed |= ImGui::SliderFloat("light strength", &directionalLight->intensity, 0.0, 1.0);
		}

		if (ImGui::TreeNode("rock color")) {
			terrianChanged |= ImGui::GradientEditor(rock_color_gradient, draggingMark, selectedMark);
		}

		if (ImGui::Button("regen")) {
			terrian->setAllDirty();
		}

		if (terrianChanged || changed) {
			save();
		}
	}

	void save() {
		json data;
		data["rock_colors"] = saveGradient(rock_color_gradient);
		data["directional_light"] = saveLight(directionalLight);
		std::ofstream outfile(fileName);
		outfile << data.dump(2) << std::endl;
		outfile.close();
	}

	void load() {
		std::ifstream f(fileName);

		if (!f) {
			return;
		}

		json data;
		f >> data;

		loadGradient(data["rock_colors"], rock_color_gradient);
		loadLight(data["directional_light"], directionalLight);
	}

	json saveLight(DirectionalLight *light) {
		json data;
		data["intensity"] = light->intensity;
		return data;
	}

	json saveGradient(ImGradient *gradient) {
		auto data = json::array();
		for (auto mark : rock_color_gradient->getMarks()) {
			auto position = mark->position;
			json gradientMark = {
			{ "r", mark->color[0] },
			{ "g", mark->color[1] },
			{ "b", mark->color[2] },
			{ "a", mark->color[3] },
			{ "position", position }
			};
			data.push_back(gradientMark);
		}
		return data;
	}

	void loadGradient(json data, ImGradient *gradient) {
		if (data.empty()) {
			return;
		}

		gradient->removeAllMarks();

		for (auto markData : data) {
			ImGradientMark *mark = new ImGradientMark();
			ImColor color = ImColor(markData["r"], markData["g"], markData["b"], (float)markData["a"]);
			gradient->addMark(markData["position"], color);
		}
	}
	void loadLight(json data, DirectionalLight *directionalLight) {
		if (data.empty()) {
			return;
		}
		directionalLight->intensity = data["intensity"];
	}
};
