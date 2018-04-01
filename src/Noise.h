#pragma once
#include "FastNoise.h"

class Noise {
public:
	FastNoise *noise = new FastNoise();
	float amplitude = 1.0;
	
	float Get(float x, float y, float z) {
		return noise->GetValue(x, y, z) * amplitude;
	}
	~Noise() {
		delete noise;
	}
};