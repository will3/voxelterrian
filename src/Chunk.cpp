#include "Chunk.h"

voxel_type Chunk::get(Coord3 coord) {
	if (data.size() == 0) {
		return 0;
	}
	int size = this->size;
	int index = coord.i * size * size + coord.j * size + coord.k;
	return data[index];
}
 
void Chunk::set(Coord3 coord, voxel_type v) {
	int index = coord.i * size * size + coord.j * size + coord.k;
	data[index] = v;
	dirty = true;
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

void Chunk::drop_node_if_needed() {
	if (mesh == 0) {
		return;
	}

	mesh->get_geometry()->drop();
	mesh->parent->remove(mesh);

	mesh = 0;
}