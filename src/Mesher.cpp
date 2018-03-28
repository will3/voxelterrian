#include <vector>
#include <algorithm>
#include <iterator>
#include "Mesher.h"
#include "Chunk.h"
#include "Raycast.h"

Voxel Mesher::get_voxel(Coord3 &coord, Chunk &chunk, Chunks &chunks) {
	int size = chunk.size;

	if (coord.i < 0 || coord.i >= size ||
		coord.j < 0 || coord.j >= size ||
		coord.k < 0 || coord.k >= size) {
		Coord3 c = coord + chunk.get_offset();
		return chunks.get(c);
	}

	return chunk.get(coord);
}

void Mesher::generate_masks(Chunk* chunk, Chunks* chunks, DirectionalLight *light) {
	if (chunk->masks.size() > 0) {
		throw std::invalid_argument("");
	}

	int size = chunk->size;

	std::unordered_map<Coord3, int> light_amount_cache;

	for (int d = 0; d < 3; d++) {
		for (int i = 0; i <= size; i++) {
			Mask *front_mask = new Mask(size, i, d, true);
			Mask *back_mask = new Mask(size, i, d, false);
			int u = (d + 1) % 3;
			int v = (d + 2) % 3;
			for (int j = 0; j < size; j++) {
				for (int k = 0; k < size; k++) {
					Coord3 coord_a = Coord3(i - 1, j, k).rotate(d);
					int a = get_voxel(coord_a, *chunk, *chunks);

					Coord3 coord_b = Coord3(i, j, k).rotate(d);
					int b = get_voxel(coord_b, *chunk, *chunks);

					bool front = a != 0;

					if ((a == 0 && b == 0) || (a != 0 && b != 0))
					{
						continue;
					}

					int aoX = a != 0 ? 1 : -1;

					Coord3 c00 = Coord3(i + aoX, j - 1, k - 1).rotate(d);
					Coord3 c01 = Coord3(i + aoX, j, k - 1).rotate(d);
					Coord3 c02 = Coord3(i + aoX, j + 1, k - 1).rotate(d);
					Coord3 c10 = Coord3(i + aoX, j - 1, k).rotate(d);
					Coord3 c12 = Coord3(i + aoX, j + 1, k).rotate(d);
					Coord3 c20 = Coord3(i + aoX, j - 1, k + 1).rotate(d);
					Coord3 c21 = Coord3(i + aoX, j, k + 1).rotate(d);
					Coord3 c22 = Coord3(i + aoX, j + 1, k + 1).rotate(d);

					int s00 = get_voxel(c00, *chunk, *chunks) > 0 ? 1 : 0;
					int s01 = get_voxel(c01, *chunk, *chunks) > 0 ? 1 : 0;
					int s02 = get_voxel(c02, *chunk, *chunks) > 0 ? 1 : 0;
					int s10 = get_voxel(c10, *chunk, *chunks) > 0 ? 1 : 0;
					int s12 = get_voxel(c12, *chunk, *chunks) > 0 ? 1 : 0;
					int s20 = get_voxel(c20, *chunk, *chunks) > 0 ? 1 : 0;
					int s21 = get_voxel(c21, *chunk, *chunks) > 0 ? 1 : 0;
					int s22 = get_voxel(c22, *chunk, *chunks) > 0 ? 1 : 0;

					int light_amount = max_light_value;
					if (light != 0) {
						Coord3 coord = front ? Coord3(i - 1, j, k) : Coord3(i, j, k);
						coord = coord.rotate(d);
						if (light_amount_cache.find(coord) == light_amount_cache.end()) {
							light_amount_cache[coord] = light->calc_light(chunk, chunks, coord);
						}

						light_amount = light_amount_cache[coord];
					}

					MaskValue v = MaskValue(a || b,
						get_ao(s10, s01, s00), get_ao(s01, s12, s02), get_ao(s12, s21, s22), get_ao(s21, s10, s20),
						light_amount);

					if (front) {
						front_mask->set(j, k, v);
					}
					else {
						back_mask->set(j, k, v);
					}
				}
			}

			chunk->masks.push_back(front_mask);
			chunk->masks.push_back(back_mask);
		}
	}

	chunk->position = float3(chunk->get_offset().i, chunk->get_offset().j, chunk->get_offset().k);

	chunk->shadow_softened = false;
}

//void Mesher::copy_mask(Mask *mask, Mesh *mesh) {
//	mask->get_quads(mesh);
//}

Coord3 Mesher::get_vertice(int i, int j, int k, int d) {
	if (d == 0) {
		return { i, j, k };
	}
	else if (d == 1) {
		return { k, i, j };
	}
	else {
		return { j, k, i };
	}
}

ao_type Mesher::get_ao(int s1, int s2, int c) {
	if (s1 && s2) {
		return 3;
	}

	return s1 + s2 + c;
}