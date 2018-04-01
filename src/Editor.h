#pragma once

#include "Noise.h"
#include "imgui.h"
#include <math.h>

class Editor {
public:
	void gui_noise(Noise *noise) {
		float gain = noise->noise->GetFractalGain();
		int octaves = noise->noise->GetFractalOctaves();
		float amplitude = noise->amplitude;
		float frequency = pow(noise->noise->GetFrequency(), 1 / 2.0);
		float lacunarity = noise->noise->GetFractalLacunarity();

		ImGui::SliderFloat("gain", &gain, 0, 1.0);
		ImGui::SliderInt("octaves", &octaves, 0, 10);
		ImGui::SliderFloat("amplitude", &amplitude, 10, 128);
		ImGui::SliderFloat("frequency", &frequency, 0, 1);
		ImGui::SliderFloat("lacunarity", &lacunarity, 0, 10.0);

		noise->noise->SetFractalGain(gain);
		noise->noise->SetFractalOctaves(octaves);
		noise->amplitude = amplitude;
		noise->noise->SetFrequency(pow(frequency, 2.0));
		noise->noise->SetFractalLacunarity(lacunarity);
	}
};
