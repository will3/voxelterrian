#include "Chunk.h"
#include "DirectionalLight.h"
#include "Chunks.h"
#include "Brush.h"
#include "ShadowMap.h"

voxel_type Chunk::get(Coord3 coord) {
	if (data.size() == 0) {
		return 0;
	}
	int size = this->size;
	int index = coord.i * size * size + coord.j * size + coord.k;
	return data[index];
}

voxel_type Chunk::get_global(Coord3 coord) {
	if (coord.i < 0 || coord.i >= size ||
		coord.j < 0 || coord.j >= size ||
		coord.k < 0 || coord.k >= size) {
		Coord3 c = coord + offset;
		return chunks->get(c);
	}

	return get(coord);
}
 
void Chunk::set(Coord3 coord, voxel_type v) {
	int index = coord.i * size * size + coord.j * size + coord.k;
	data[index] = v;
	dirty = true;
	needs_calc_light = true;
}

Chunk::Chunk(int size, Coord3 origin)
{
	this->origin = origin;
	this->size = size;
	this->offset = origin * size;

	data.resize(size * size * size);

	shadow_map = new ShadowMap();
}

Chunk::Chunk(int size)
{
	this->size = size;

	data.resize(size * size * size);

	shadow_map = new ShadowMap();
}

Chunk::~Chunk()
{
	delete height_map;
}

bool Chunk::get_light_raw(int i, int j, int k) {
	int index = i * size * size + j * size + k;
	if (raw_light_map.find(index) == raw_light_map.end()) {
		return false;
	}
	return raw_light_map[index];
}

bool Chunk::has_light_raw(int i, int j, int k) {
	int index = i * size * size + j * size + k;
	if (raw_light_map.find(index) == raw_light_map.end()) {
		return false;
	}
	return true;
}

int Chunk::get_light(Coord3 coord) {
	return shadow_map->get(coord);
	//if (coord.i < 0 || coord.i >= size ||
	//	coord.j < 0 || coord.j >= size ||
	//	coord.k < 0 || coord.k >= size) {
	//	Coord3 c = coord + offset;
	//	return chunks->get_light(c);
	//}

	//int index = coord.i * size * size + coord.j * size + coord.k;
	//if (raw_light_map.find(index) == raw_light_map.end()) {
	//	return 0;
	//}
	//return raw_light_map[index] ? max_light_value : 0;
}

void Chunk::calc_light_if_needed(DirectionalLight * light)
{
	if (needs_calc_light) {
		calc_light(light);
		needs_calc_light = false;
		needs_smooth_light = true;
	}
}

void Chunk::calc_light(DirectionalLight * light) {
	if (shadow_map != 0) {
		delete shadow_map;
	}

	shadow_map = new ShadowMap();
	shadow_map->calc_shadow(this, light);

	light_calculated = true;
}

void Chunk::smooth_light_if_needed(DirectionalLight * light, Brush *brush)
{
	if (needs_smooth_light) {
		smooth_light(light, brush);
		needs_smooth_light = false;
	}
}

void Chunk::smooth_light(DirectionalLight * light, Brush *brush)
{
	int index = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				Coord3 coord = { i,j,k };
				voxel_type v = data[index];
				if (v != 0) {
					smooth_light_map[index] = get_light_raw(i, j, k) ? 
						max_light_value : 
						0.0;
					//float light = 0;

					//float count = 0;
					//for (auto kv : brush->profile) {
					//	Coord3 c = coord + Coord3(kv.first);

					//	if (!has_light_raw(c.i, c.j, c.k)) {
					//		continue;
					//	}

					//	float v = kv.second;


					//	light += (get_light_raw(c.i, c.j, c.k) ? 15.0 : 0.0) * v;
					//	count += 1.0;
					//}

					//light /= count;

					//smooth_light_map[index] = floor(light);
				}

				index++;
			}
		}
	}
}