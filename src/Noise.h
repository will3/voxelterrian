#pragma once
#include "FastNoise.h"

class Noise {
private:

public:
	FastNoise * noise = new FastNoise();
	float amplitude = 1.0;
	float yScale = 1.0;
	float maxAmplitude = 1.0;

	~Noise() {
		delete noise;
	}
};