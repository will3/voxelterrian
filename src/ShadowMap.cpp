#include "ShadowMap.h"

void ShadowMap::calc_shadow(Chunk * chunk, DirectionalLight *light)
{
	for (int d = 0; d < 3; d++) {
		for (int i = 0; i <= CHUNK_SIZE; i++) {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				for (int k = 0; k < CHUNK_SIZE; k++) {
					Coord3 coord_front = Coord3(i - 1, j, k).rotate(d);
					int a = chunk->get_global(coord_front);

					Coord3 coord_back = Coord3(i, j, k).rotate(d);
					int b = chunk->get_global(coord_back);

					if ((a == 0 && b == 0) || (a != 0 && b != 0))
					{
						continue;
					}

					bool front = a != 0;

					if (i == 0 && front) {
						continue;
					}

					if (i == CHUNK_SIZE && !front) {
						continue;
					}

					int amount = light->calc_light(chunk, chunk->chunks, d, i, j, k, front);

					Coord3 coord = front ? coord_front : coord_back;
					set(coord, d, front, amount);
				}
			}
		}
	}
}

void ShadowMap::set(Coord3 coord, int d, int front, int v) {
	total[coord] += v;
	count[coord] += 1.0;
}

int ShadowMap::get(Coord3 coord) {
	return total[coord] / count[coord];
}
