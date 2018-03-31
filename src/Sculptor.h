#pragma once
#include "Chunk.h"

class Sculptor {
public:
	static void rasterize_height(Chunk *chunk, FastNoise *height_noise);
};