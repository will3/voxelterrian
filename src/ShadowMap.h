#pragma once

#include "Chunk.h"
#include "DirectionalLight.h"
#include "types.h"
#include "Brush.h"

class ShadowMap {
private:
	std::unordered_map<Coord3, std::unordered_map<int, int>> data;
	std::unordered_map<Coord3, float> average_data;

	void set(Coord3 coord, int d, int front, int v);
	int get(Coord3 coord, int d, int front);

public:
	void calc_shadow(Chunk *chunk, DirectionalLight *light);
	int get(Coord3 coord);
};