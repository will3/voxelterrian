#pragma once
#include "FastNoise.h"

class Noise {
private:
	FastNoise * noise = new FastNoise();

public:
	float amplitude = 1.0;
	float y_scale = 1.0;
	FastNoise *get_noise() { return noise; }

	~Noise() {
		delete noise;
	}
};