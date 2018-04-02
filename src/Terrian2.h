#pragma once

#include "FastNoise.h"
#include "Noise.h"
#include "Density_SSBO.h"
#include "types.h"
#include "Entity.h"
#include "ComputeShader.h"
#include "MarchingCube.h"

class Terrian2 : public Entity {
public:
	Noise *height_noise = new Noise();
	Noise *canyon_noise = new Noise();

	Terrian2() {
		height_noise->amplitude = 128.0;
		height_noise->get_noise()->SetFractalOctaves(5);
		height_noise->y_scale = 0.4;

		canyon_noise->get_noise()->SetFractalType(FastNoise::RigidMulti);
		canyon_noise->get_noise()->SetFractalOctaves(1);
	}

	void start() {
		draw(Coord3(0, 0, 0), 32);
	}

	void draw(Coord3 start_coord, int size) {
		Chunk *chunk = new Chunk();
		chunk->set({ 0, 0, 0 }, 1);
		std::vector<Vertex> vertices;
		std::vector<int> indices;
		generate_geometry(chunk, vertices, indices);
	}

	~Terrian2() {
		delete height_noise;
		delete canyon_noise;
	}
};