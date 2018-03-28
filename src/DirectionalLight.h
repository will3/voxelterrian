#pragma once
#include "types.h"
#include "Chunk.h"
#include "Chunks.h"

class DirectionalLight
{
public:
	float3 direction;
	DirectionalLight();
	~DirectionalLight();

	int calc_light(Chunk * chunk, Chunks * chunks, Coord3 coord);

	int calc_light(Chunk *chunk, Chunks *chunks, int d, int i, int j, int k, bool front);
};

