#pragma once
#include <vector>
#include "types.h"
#include "Mask.h"
#include "NoiseMap2.h"
#include "Mesh.h"

typedef int Voxel;

class Chunk
{
private:
	std::vector<Voxel> data;
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
	float3 position;

	Voxel get(Coord3 coord);
	void set(Coord3 coord, Voxel v);

	// meshing
	std::vector<Mask *> masks;
	kst::Mesh *mesh = 0;
	//IMeshSceneNode *node = 0;

	// terrian
	NoiseMap2 *height_map;
	bool rasterized = false;
	int distance_from_player = 0;

	void drop_node_if_needed();
};

