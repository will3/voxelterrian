#include "Chunk.h"
#include "DirectionalLight.h"
#include "Chunks.h"
#include "Brush.h"
#include "VoxelShadowMap.h"

voxel_type Chunk::get(Coord3 coord) {
	int index = coord.i * CHUNK_SIZE * CHUNK_SIZE + coord.j * CHUNK_SIZE + coord.k;
	return data[index];
}

voxel_type Chunk::get(Coord3 coord, voxel_type out_of_bound_value) {
	if (coord.i < 0 || coord.i >= CHUNK_SIZE ||
		coord.j < 0 || coord.j >= CHUNK_SIZE ||
		coord.k < 0 || coord.k >= CHUNK_SIZE) {
		return out_of_bound_value;
	}

	int index = coord.i * CHUNK_SIZE * CHUNK_SIZE + coord.j * CHUNK_SIZE + coord.k;
	return data[index];
}

voxel_type Chunk::get_global(Coord3 coord) {
	if (coord.i < 0 || coord.i >= CHUNK_SIZE ||
		coord.j < 0 || coord.j >= CHUNK_SIZE ||
		coord.k < 0 || coord.k >= CHUNK_SIZE) {
		Coord3 c = coord + offset;
		return chunks->get(c);
	}

	return get(coord);
}
 
inline int get_index(int i, int j, int k) {
	return i * CHUNK_SIZE * CHUNK_SIZE + j * CHUNK_SIZE + k;
}
void Chunk::set(Coord3 coord, voxel_type v) {
	int index = get_index(coord.i, coord.j, coord.k);
	data[index] = v;
	dirty = true;
	needs_calc_light = true;
}

void Chunk::set_color(Coord3 coord, uint8_t r, uint8_t g, uint8_t b)
{
	int index = get_index(coord.i, coord.j, coord.k) * 3;
	data_color[index] = r;
	data_color[index + 1] = g;
	data_color[index + 2] = b;
}

void Chunk::get_color(Coord3 coord, uint8_t &r, uint8_t &g, uint8_t &b) {
	int index = get_index(coord.i, coord.j, coord.k) * 3;
	r = data_color[index];
	g = data_color[index + 1];
	b = data_color[index + 2];
}

Chunk::Chunk(Coord3 origin)
{
	this->origin = origin;
	this->offset = origin * CHUNK_SIZE;

	data.resize(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
	data_color.resize(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 3);
	shadow_map = new VoxelShadowMap();
}

Chunk::~Chunk()
{
	delete shadow_map;
}

bool Chunk::get_light_raw(int i, int j, int k) {
	int index = i * CHUNK_SIZE * CHUNK_SIZE + j * CHUNK_SIZE + k;
	if (raw_light_map.find(index) == raw_light_map.end()) {
		return false;
	}
	return raw_light_map[index];
}

bool Chunk::has_light_raw(int i, int j, int k) {
	int index = i * CHUNK_SIZE * CHUNK_SIZE + j * CHUNK_SIZE + k;
	if (raw_light_map.find(index) == raw_light_map.end()) {
		return false;
	}
	return true;
}

int Chunk::get_light(Coord3 coord) {
	if (this->light_calculated) {
		return shadow_map->get(coord);
	}

	return max_light_value;
}

void Chunk::calc_light_if_needed(DirectionalLight * light)
{
	if (needs_calc_light) {
		calc_light(light);
		needs_calc_light = false;
	}
}

void Chunk::calc_light(DirectionalLight * light) {
	if (shadow_map != 0) {
		delete shadow_map;
	}

	shadow_map = new VoxelShadowMap();
	shadow_map->calc_shadow(this, light);

	light_calculated = true;
}