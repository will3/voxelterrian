#pragma once
#include "Chunk.h"
#include "Noise.h"

class Sculptor {
public:
	static void rasterize_height(Chunk *chunk, Noise *height_noise);
};