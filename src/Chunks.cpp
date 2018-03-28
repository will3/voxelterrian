#include "Chunks.h"

Chunks::Chunks(int size)
{
	this->size = size;
}


Chunks::~Chunks()
{
}

inline int fast_floor(int x, int y) {
	return x / y - (x % y < 0);
}

Coord3 Chunks::get_origin(Coord3 coord) {
	return { 
		fast_floor(coord.i, size),
		fast_floor(coord.j, size),
		fast_floor(coord.k, size)
	};
}

bool Chunks::has_chunk(Coord3& origin) {
	return data[origin.i][origin.j][origin.k] != 0;
}

Voxel Chunks::get(Coord3 coord) {
	Coord3 origin = get_origin(coord);

	if (!has_chunk(origin)) {
		return 0;
	}

	return data[origin.i][origin.j][origin.k]->get(coord - origin * size);
}

void Chunks::set(Coord3 coord, Voxel v) {
	Coord3 origin = get_origin(coord);

	get_or_create_chunk(origin)->set(coord - origin * size, v);
}

bool Chunks::has(Coord3 coord) {
	return coords.find(coord) != coords.end();
}

Chunk* Chunks::get_or_create_chunk(Coord3 origin) {
	Chunk *chunk = data[origin.i][origin.j][origin.k];
	if (chunk == 0) {
		chunk = data[origin.i][origin.j][origin.k] = new Chunk(size, origin);
		coords.insert(origin);
	}
	return chunk;
}

bool Chunks::remove_chunk(Coord3 origin) {
	Chunk *chunk = data[origin.i][origin.j][origin.k];
	if (chunk == 0) {
		return false;
	}
	chunk->drop_node_if_needed();
	coords.erase(origin);
	delete chunk;
	data[origin.i][origin.j][origin.k] = 0;
	return true;
}