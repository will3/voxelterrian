#pragma once

#include "Noise.h"
#include "imgui.h"
#include <math.h>

class Editor {
public:
	bool show(Noise *height_noise, EditorCameraControl *control) {
		bool result = false;
		ImGui::Begin("Editor");
		camera_speed(control);
		result |= noise(height_noise);
		ImGui::End();
		return result;
	}

	void camera_speed(EditorCameraControl *control) {
		int camera_speed = control->speed / 4;
		ImGui::Text("Generation Speed");
		ImGui::RadioButton("0", &camera_speed, 0); ImGui::SameLine();
		ImGui::RadioButton("1", &camera_speed, 1); ImGui::SameLine();
		ImGui::RadioButton("2", &camera_speed, 2); ImGui::SameLine();
		ImGui::RadioButton("4", &camera_speed, 4); ImGui::SameLine();
		ImGui::RadioButton("8", &camera_speed, 8); ImGui::SameLine();
		ImGui::RadioButton("16", &camera_speed, 16);
		control->speed = camera_speed * 4;
	}

	bool noise(Noise *noise) {
		float gain = noise->noise->GetFractalGain();
		int octaves = noise->noise->GetFractalOctaves();
		float amplitude = noise->amplitude;
		float frequency = pow(noise->noise->GetFrequency(), 1 / 2.0);
		float lacunarity = noise->noise->GetFractalLacunarity();

		bool result = false;
		result |= ImGui::SliderFloat("gain", &gain, 0, 1.0);
		result |= ImGui::SliderInt("octaves", &octaves, 0, 10);
		result |= ImGui::SliderFloat("amplitude", &amplitude, 10, 128);
		result |= ImGui::SliderFloat("frequency", &frequency, 0, 1);
		result |= ImGui::SliderFloat("lacunarity", &lacunarity, 0, 10.0);

		noise->noise->SetFractalGain(gain);
		noise->noise->SetFractalOctaves(octaves);
		noise->amplitude = amplitude;
		noise->noise->SetFrequency(pow(frequency, 2.0));
		noise->noise->SetFractalLacunarity(lacunarity);

		return result;
	}
};
