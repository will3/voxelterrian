#pragma once

#include "FastNoise.h"
#include "Noise.h"
#include "Density_SSBO.h"
#include "types.h"
#include "Entity.h"

class Terrian2 : public Entity {
	Noise *height_noise = new Noise();
	Noise *canyon_noise = new Noise();

	Terrian2() {
		height_noise->amplitude = 128.0;
		height_noise->get_noise()->SetFractalOctaves(5);
		height_noise->y_scale = 0.4;

		canyon_noise->get_noise()->SetFractalType(FastNoise::RigidMulti);
		canyon_noise->get_noise()->SetFractalOctaves(1);
	}

	void draw(Coord3 start_coord, int size) {
		Density_SSBO *ssbo = new Density_SSBO();

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				for (int k = 0; k < size; k++) {
					Coord3 coord = start_coord + Coord3(i, j, k);
					float fractal = height_noise->get_noise()->GetSimplexFractal(coord.i, coord.j * height_noise->y_scale, coord.k);
					float density = fractal * height_noise->amplitude - coord.j;
					if (density > 1.0) density = 1.0;
					if (density < 0.0) density = 0.0;
					ssbo->density.push_back(density);
				}
			}
		}

		ssbo->load();
	}

	~Terrian2() {
		delete height_noise;
		delete canyon_noise;
	}
};