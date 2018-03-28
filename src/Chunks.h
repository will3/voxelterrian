#pragma once

#include "Chunk.h"
#include <map>
#include <unordered_map>
#include <string>
#include <unordered_set>

class ChunksVisitor {
public:
	virtual void visit(Chunk *chunk) = 0;
};

class Chunks
{
private:
	Coord3 get_origin(Coord3 coord);
	std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, Chunk*>>> data;
	std::unordered_set<Coord3> coords;
public:
	int size;

	Chunks(int size = 32);
	~Chunks();
	Voxel get(Coord3 coord);
	void set(Coord3 coord, Voxel v);
	bool has(Coord3 coord);
	Chunk* get_or_create_chunk(Coord3 origin);
	bool remove_chunk(Coord3 origin);
	bool has_chunk(Coord3& origin);

	void visit_chunks(ChunksVisitor &visitor) {
		for (auto coord : coords) {
			Chunk *chunk = data[coord.i][coord.j][coord.k];
			visitor.visit(chunk);
		}
	}

	int count_chunks() {
		return coords.size();
	}
};
