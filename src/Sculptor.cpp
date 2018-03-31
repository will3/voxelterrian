#include "Sculptor.h"

FastNoise * terrian_noise = new FastNoise();
FastNoise * height_noise = new FastNoise();

int get_terrian(int i, int j, int k) {
	//float height = height_noise->GetSimplex(i, k);
	//float gradient = (1 - j / 128.0);
	float height = terrian_noise->GetSimplexFractal(i, j * 0.2, k) * 128.0;
	float v = j <= height ? 1 : 0;
	return v > 0.5 ? 1 : 0;
}

void Sculptor::rasterize_height(Chunk * chunk, FastNoise * height_noise) {
	Coord3 offset = chunk->get_offset();
	int chunk_size = chunk->size;

	for (int i = 0; i < chunk_size; i++) {
		for (int j = 0; j < chunk_size; j++) {
			for (int k = 0; k < chunk_size; k++) {
				Coord3 coord = Coord3(i, j, k) + offset;
				chunk->set({ i, j, k }, get_terrian(coord.i, coord.j, coord.k));
			}
		}
	}
}