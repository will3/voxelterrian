#include "Chunks.h"

inline int fast_floor(int x, int y) {
	return x / y - (x % y < 0);
}

Chunks::Chunks(int size)
{
	this->size = size;
}

Chunks::~Chunks()
{
}

Coord3 Chunks::get_origin(Coord3 coord) {
	return { 
		fast_floor(coord.i, size),
		fast_floor(coord.j, size),
		fast_floor(coord.k, size)
	};
}

bool Chunks::has_chunk(Coord3 origin) {
	return coords.find(origin) != coords.end();
}

Chunk * Chunks::get_chunk(Coord3 origin)
{
	return map[origin];
}

std::unordered_set<Coord3> Chunks::get_coords()
{
	return coords;
}

voxel_type Chunks::get(Coord3 coord) {
	Coord3 origin = get_origin(coord);

	if (!has_chunk(origin)) {
		return 0;
	}

	return map[origin]->get(coord - origin * size);
}

void Chunks::set(Coord3 coord, voxel_type v) {
	Coord3 origin = get_origin(coord);

	get_or_create_chunk(origin)->set(coord - origin * size, v);
}

Chunk* Chunks::get_or_create_chunk(Coord3 origin) {
	if (!has_chunk(origin)) {
		map[origin] = new Chunk(size, origin);
		map[origin]->chunks = this;
		coords.insert(origin);
	}

	return map[origin];
}

void Chunks::remove_chunk(Coord3 origin) {
	Chunk *chunk = map[origin];
	chunk->drop_node_if_needed();
	map.erase(origin);
	coords.erase(origin);
	delete chunk;
}