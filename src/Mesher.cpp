#include <vector>
#include <algorithm>
#include <iterator>
#include "Mesher.h"
#include "Chunk.h"
#include "Raycast.h"
#include "Material.h"
#include "Mesh.h"

voxel_type Mesher::get_voxel(Coord3 &coord, Chunk &chunk, Chunks &chunks) {
	int size = chunk.size;

	if (coord.i < 0 || coord.i >= size ||
		coord.j < 0 || coord.j >= size ||
		coord.k < 0 || coord.k >= size) {
		Coord3 c = coord + chunk.get_offset();
		return chunks.get(c);
	}

	return chunk.get(coord);
}

void Mesher::gen_masks(Chunk* chunk, Chunks* chunks, DirectionalLight *light) {
	if (chunk->masks.size() > 0) {
		throw std::invalid_argument("");
	}

	int size = chunk->size;

	std::unordered_map<Coord3, int> light_amount_cache;

	for (int d = 0; d < 3; d++) {
		for (int i = 0; i <= size; i++) {
			Mask front_mask = Mask(size, i, d, true);
			Mask back_mask = Mask(size, i, d, false);
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
							Coord3 chunks_coord = coord + chunk->get_offset();
							light_amount_cache[coord] = light->calc_light(chunks, chunks_coord) ? 15 : 0;
						}

						light_amount = light_amount_cache[coord];
					}

					MaskValue v = MaskValue(a || b,
						get_ao(s10, s01, s00), get_ao(s01, s12, s02), get_ao(s12, s21, s22), get_ao(s21, s10, s20),
						light_amount);

					if (front) {
						front_mask.set(j, k, v);
					}
					else {
						back_mask.set(j, k, v);
					}
				}
			}

			chunk->masks.push_back(front_mask);
			chunk->masks.push_back(back_mask);
		}
	}

	chunk->position = { chunk->get_offset().i, chunk->get_offset().j, chunk->get_offset().k };

	chunk->shadow_softened = false;
}

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

bool Mesher::stop_merge(MaskValue & c, MaskValue & next) {
	return next.v != c.v || next.has_ao() || next.lighting != c.lighting;
}

void Mesher::gen_geometry(Mask& mask, Geometry *geometry, int x, int y, int w, int h, int ao0, int ao1, int ao2, int ao3, int l) {
	bool front = mask.front;
	float ao_strength = 0.1f;
	auto &vertices = geometry->vertices;
	auto &colors = geometry->colors;
	auto &lighting = geometry->lighting;
	auto &indices = geometry->indices;
	int i = mask.i;
	int d = mask.d;
	int index = geometry->num_vertices();

	Coord3 v0 = Coord3(i, x, y).rotate(d);
	Coord3 v1 = Coord3(i, x + w, y).rotate(d);
	Coord3 v2 = Coord3(i, x + w, y + h).rotate(d);
	Coord3 v3 = Coord3(i, x, y + h).rotate(d);

	vertices.insert(vertices.end(), { v0.i, v0.j, v0.k });
	vertices.insert(vertices.end(), { v1.i, v1.j, v1.k });
	vertices.insert(vertices.end(), { v2.i, v2.j, v2.k });
	vertices.insert(vertices.end(), { v3.i, v3.j, v3.k });

	colors.insert(colors.end(), { 255, 255, 255 });
	colors.insert(colors.end(), { 255, 255, 255 });
	colors.insert(colors.end(), { 255, 255, 255 });
	colors.insert(colors.end(), { 255, 255, 255 });

	float light_f = (1.0f - (ao0 / 3.0f * ao_strength)) * (l / 15.0f);
	int light = floor(light_f * 16);

	lighting.insert(lighting.end(), { light, light, light, light });

	if (front) {
		indices.push_back(index);
		indices.push_back(index + 1);
		indices.push_back(index + 2);
		indices.push_back(index + 2);
		indices.push_back(index + 3);
		indices.push_back(index);
	}
	else {
		indices.push_back(index + 2);
		indices.push_back(index + 1);
		indices.push_back(index);
		indices.push_back(index);
		indices.push_back(index + 3);
		indices.push_back(index + 2);
	}
}

void Mesher::gen_geometry(Mask& mask, Geometry *geometry) {
	int n = 0;
	MaskValue c;
	int w, h;
	int size = mask.size;
	auto data = mask.data;

	for (int j = 0; j < size; j++) {
		for (int i = 0; i < size; ) {
			c = data[n];

			if (c.v == 0) {
				i++;
				n++;
				continue;
			}

			// Check AO
			if (c.has_ao()) {
				gen_geometry(mask, geometry, j, i, 1, 1, c.ao0, c.ao1, c.ao2, c.ao3, c.lighting);
				i++;
				n++;
				continue;
			}

			int lighting = c.lighting;

			// Compute width
			for (w = 1; i + w < size; ++w) {
				MaskValue &next = data[n + w];
				if (stop_merge(c, next)) {
					break;
				}
			}

			// Compute height
			bool done = false;
			for (h = 1; j + h < size; h++) {
				for (int k = 0; k < w; k++) {
					MaskValue &next = data[n + k + h * size];
					if (stop_merge(c, next)) {
						done = true;
						break;
					}
				}
				if (done) {
					break;
				}
			}

			// Add Quad
			gen_geometry(mask, geometry, j, i, h, w, 0, 0, 0, 0, lighting);

			//Zero-out mask
			for (int l = 0; l < h; l++) {
				for (int k = 0; k < w; k++) {
					data[n + k + l * size] = 0;
				}
			}

			i += w; n += w;
		}
	}
}

void Mesher::gen_geometry(Chunk* chunk) {
	chunk->geometry = new Geometry();
	for (Mask& mask : chunk->masks) {
		gen_geometry(mask, chunk->geometry);
	}
	chunk->masks.clear();
}