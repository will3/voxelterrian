#include "Chunk.h"
#include "DirectionalLight.h"
#include "Chunks.h"

voxel_type Chunk::get(Coord3 coord) {
	if (data.size() == 0) {
		return 0;
	}
	int size = this->size;
	int index = coord.i * size * size + coord.j * size + coord.k;
	return data[index];
}

inline voxel_type Chunk::get_global(Coord3 coord) {
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
}

float Chunk::get_light(int i, int j, int k)
{
	int index = i * size * size + j * size + k;
	if (has_light_map.find(index) == has_light_map.end()) {
		return false;
	}
	return has_light_map[index] ? 15.0 : 0.0;
}

Chunk::Chunk(int size, Coord3 origin)
{
	this->origin = origin;
	this->size = size;
	this->offset = origin * size;

	data.resize(size * size * size);
}

Chunk::Chunk(int size)
{
	this->size = size;

	data.resize(size * size * size);
}

Chunk::~Chunk()
{
	delete height_map;
}

inline void Chunk::calc_light(DirectionalLight * light) {
	int index = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				voxel_type v = data[index];
				if (v != 0) {
					has_light_map[index] = light->calc_light(this->chunks, Coord3(i, j, k) + offset);
				}
				index++;
			}
		}
	}
}

void Chunk::smooth_light(DirectionalLight * light)
{
	int index = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				voxel_type v = data[index];
				if (v != 0) {
					float light = 0;
					light += get_light(i, j, k) * 1.0;

					smooth_light_map[index] = floor(light);
				}

				index++;
			}
		}
	}
}

void Chunk::drop_node_if_needed() {
	if (mesh == 0) {
		return;
	}

	mesh->get_geometry()->drop();
	mesh->parent->remove(mesh);

	mesh = 0;
}