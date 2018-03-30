#pragma once
#include <vector>
#include "types.h"
#include "Mask.h"
#include "NoiseMap2.h"
#include "Mesh.h"

typedef GLbyte voxel_type;

class Chunk
{
private:
	std::vector<voxel_type> data;
	Coord3 origin;
	Coord3 offset;

public:
	Chunk(int size, Coord3 origin);
	Chunk(int size = 32);
	~Chunk();
	Coord3& get_origin() { return origin; }
	Coord3& get_offset() { return offset; }
	int size;
	bool dirty;

	bool shadow_softened = false;
	glm::vec3 position;

	voxel_type get(Coord3 coord);
	void set(Coord3 coord, voxel_type v);

	// meshing
	std::vector<Mask> masks;
	Geometry *geometry = 0;
	bool geometry_ready = false;
	Mesh *mesh = 0;

	// terrian
	NoiseMap2 *height_map = 0;
	bool rasterized = false;
	int distance_from_player = 0;

	void drop_node_if_needed();
};

