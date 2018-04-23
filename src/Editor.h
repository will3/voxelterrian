#pragma once

#include "Noise.h"
#include "imgui.h"
#include <math.h>
#include "imgui_color_gradient.h"
#include <json.hpp>
#include <iostream>
#include <fstream>  
#include "Terrian2.h"
#include "Noise.h"

ImGradientMark *draggingMark;
ImGradientMark *selectedMark;

using json = nlohmann::json;

class Editor {
public:
	ImGradient *rock_color_gradient;
	DirectionalLight *directionalLight;
	std::string fileName = "editor";
	Terrian2 *terrian;
	bool terrianChanged = false;
	std::vector<ImVec2> points = { ImVec2(0, 0), ImVec2(5, 5), ImVec2(10.0, 10.0) };
	void draw() {
		terrianChanged = false;
		bool changed = false;

		if (ImGui::TreeNode("height")) {
			terrianChanged |= drawNoise(terrian->heightNoise);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("lighting")) {
			changed |= ImGui::SliderFloat("light strength", &directionalLight->intensity, 0.0, 1.0);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("rock color")) {
			terrianChanged |= ImGui::GradientEditor(rock_color_gradient, draggingMark, selectedMark);
			ImGui::TreePop();
		}

		if (ImGui::Button("regen")) {
			terrian->setAllDirty();
		}

		if (terrianChanged || changed) {
			save();
		}
	}

	bool drawNoise(Noise *noise) {
		bool changed = false;
		FastNoise *rawNoise = noise->noise;

		float frequency = rawNoise->GetFrequency();
		float lacunarity = rawNoise->GetFractalLacunarity();
		float gain = rawNoise->GetFractalGain();
		int octaves = rawNoise->GetFractalOctaves();

		changed |= ImGui::SliderFloat("amplitude", &noise->amplitude, 0, noise->maxAmplitude);
		changed |= ImGui::SliderFloat("frequency", &frequency, 0.001, 0.1, 0, 10.0);
		changed |= ImGui::SliderFloat("y-scale", &noise->yScale, 0.0, 1.0);
		changed |= ImGui::SliderFloat("lacunarity", &lacunarity, 0.0, 10.0);
		changed |= ImGui::SliderFloat("gain", &gain, 0.0, 1.0);
		changed |= ImGui::SliderInt("octaves", &octaves, 1, 10);

		rawNoise->SetFrequency(frequency);
		rawNoise->SetFractalLacunarity(lacunarity);
		rawNoise->SetFractalGain(gain);
		rawNoise->SetFractalOctaves(octaves);

		return changed;
	}

	void save() {
		json data;
		data["rock_colors"] = saveGradient(rock_color_gradient);
		data["directional_light"] = saveLight(directionalLight);
		data["height_noise"] = saveNoise(terrian->heightNoise);
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
		loadNoise(data["height_noise"], terrian->heightNoise);
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

	json saveNoise(Noise *noise) {
		json data;
		data["amplitude"] = noise->amplitude;
		data["frequency"] = noise->noise->GetFrequency();
		data["y-scale"] = noise->yScale;
		data["lacunarity"] = noise->noise->GetFractalLacunarity();
		data["gain"] = noise->noise->GetFractalGain();
		data["octaves"] = noise->noise->GetFractalOctaves();
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

	void loadNoise(json data, Noise *noise) {
		if (data.empty()) {
			return;
		}
		noise->amplitude = data["amplitude"];
		noise->noise->SetFrequency(data["frequency"]);
		noise->yScale = data["y-scale"];
		noise->noise->SetFractalLacunarity(data["lacunarity"]);
		noise->noise->SetFractalGain(data["gain"]);
		noise->noise->SetFractalOctaves(data["octaves"]);
	}
};
