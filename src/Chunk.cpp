#include "Chunk.h"
#include "DirectionalLight.h"
#include "Chunks.h"
#include "Brush.h"
#include "ShadowMap.h"

voxel_type Chunk::get(Coord3 coord) {
	if (data.size() == 0) {
		return 0;
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
 
void Chunk::set(Coord3 coord, voxel_type v) {
	int index = coord.i * CHUNK_SIZE * CHUNK_SIZE + coord.j * CHUNK_SIZE + coord.k;
	data[index] = v;
	dirty = true;
	needs_calc_light = true;
}

Chunk::Chunk(Coord3 origin)
{
	this->origin = origin;
	this->offset = origin * CHUNK_SIZE;

	data.resize(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);

	shadow_map = new ShadowMap();
}

Chunk::~Chunk()
{
	delete height_map;
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