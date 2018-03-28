#pragma once
#include "types.h"
#include <map>
#include <vector>
#include "FastNoise.h"

class NoiseMap2
{
private:
	std::vector<float> data;
	FastNoise *noise;
	void set(int i, int j, float v);
	float get(int i, int j);
	Coord2 offset;

public:
	NoiseMap2(FastNoise *noise, int size, Coord2 offset, int scale);
	~NoiseMap2();
	int size;
	int scale = 1;
	float amplitude = 1;
	bool populated = false;

	float sample(Coord2 coord);
	void populate();
};