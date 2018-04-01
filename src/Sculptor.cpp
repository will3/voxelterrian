#include "Sculptor.h"
#include "Field3.h"

void Sculptor::rasterize_height(Chunk * chunk, Noise * height_noise) {
	Coord3 offset = chunk->get_offset();

	int noise_size = 17;
	Field3<float> field = Field3<float>(noise_size);
	for (int i = 0; i < noise_size; i++) {
		for (int j = 0; j < noise_size; j++) {
			for (int k = 0; k < noise_size; k++) {
				Coord3 coord = Coord3(i, j, k) * 2 + offset;

				float density = height_noise->noise->GetSimplexFractal(coord.i, coord.j * 0.0, coord.k) * 128.0 - coord.j;
				//float density = height_noise->noise->GetSimplexFractal(coord.i, coord.j * 0.4, coord.k) * 128.0 - coord.j;
				field.set(i, j, k, density);
			}
		}
	}

	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				float density = field.sample(i * 0.5, j * 0.5, k * 0.5);
				chunk->set({ i, j, k }, density > 0.5 ? 1 : 0);
			}
		}
	}
}