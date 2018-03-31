#pragma once

#include "Chunk.h"
#include <map>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <functional>

class Chunks
{
private:
	Coord3 get_origin(Coord3 coord);
	std::unordered_map<Coord3, Chunk *> map;
	std::unordered_set<Coord3> coords;
public:
	int size;

	Chunks(int size = 32);
	~Chunks();
	voxel_type get(Coord3 coord);
	void set(Coord3 coord, voxel_type v);
	Chunk* get_or_create_chunk(Coord3 origin);
	void delete_chunk(Coord3 origin);
	bool has_chunk(Coord3 origin);
	Chunk *get_chunk(Coord3 origin);

	std::unordered_set<Coord3> get_coords();

	int get_light(Coord3 coord);
};

