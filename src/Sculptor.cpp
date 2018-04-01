#include "Sculptor.h"
#include "Field3.h"

FastNoise * terrian_noise = new FastNoise();
FastNoise * height_noise = new FastNoise();

//int get_terrian(int i, int j, int k) {
//	//float height = height_noise->GetSimplex(i, k);
//	//float gradient = (1 - j / 128.0);
//	float height = ;
//	float v = j <= height ? 1 : 0;
//	return v > 0.5 ? 1 : 0;
//}

void Sculptor::rasterize_height(Chunk * chunk, FastNoise * height_noise) {
	Coord3 offset = chunk->get_offset();

	int noise_size = 16;
	Field3<float> *field = new Field3<float>(noise_size);
	for (int i = 0; i < noise_size; i++) {
		for (int j = 0; j < noise_size; j++) {
			for (int k = 0; k < noise_size; k++) {
				Coord3 coord = Coord3(i, j, k) * 2 + offset;
				float density = terrian_noise->GetSimplexFractal(coord.i, coord.j * 0.4, coord.k) * 128.0 - coord.j;
				field->set(i, j, k, density);
			}
		}
	}

	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				float density = field->sample(i * 0.5, j * 0.5, k * 0.5);
				chunk->set({ i, j, k }, density > 0.5 ? 1 : 0);
			}
		}
	}
}