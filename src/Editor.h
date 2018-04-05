#pragma once

#include "Noise.h"
#include "imgui.h"
#include <math.h>
#include "imgui_color_gradient.h"
#include <json.hpp>
#include <iostream>
#include <fstream>  

ImGradientMark *draggingMark;
ImGradientMark *selectedMark;

using json = nlohmann::json;

class Editor {
public:
	ImGradient *rock_color_gradient;
	std::string fileName = "editor";

	void draw() {
		bool changed = false;
		ImGui::Text("rock color");
		changed |= ImGui::GradientEditor(rock_color_gradient, draggingMark, selectedMark);
		if (changed) {
			save();
		}
	}

	void save() {
		json data;
		data["rock_colors"] = saveGradient(rock_color_gradient);

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
		gradient->removeAllMarks();

		for (auto markData : data) {
			ImGradientMark *mark = new ImGradientMark();
			ImColor color = ImColor(markData["r"], markData["g"], markData["b"], (float)markData["a"]);
			gradient->addMark(markData["position"], color);
		}
	}
};
