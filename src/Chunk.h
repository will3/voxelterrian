#pragma once
#include <vector>
#include "types.h"
#include "Mask.h"
#include "NoiseMap2.h"
#include "Mesh.h"
#include <unordered_map>

#define CHUNK_SIZE 32

typedef GLbyte voxel_type;
class DirectionalLight;
class Chunks;
class Brush;
class ShadowMap;

class Chunk
{
private:
	std::vector<voxel_type> data;
	Coord3 origin;
	Coord3 offset;

	std::unordered_map<int, bool> raw_light_map;
	std::unordered_map<int, int> smooth_light_map;
	ShadowMap *shadow_map;

public:
	Chunk(Coord3 origin);
	~Chunk();
	Coord3& get_origin() { return origin; }
	Coord3& get_offset() { return offset; }
	bool dirty;
	Chunks *chunks;

	glm::vec3 position;

	voxel_type get(Coord3 coord);
	voxel_type get_global(Coord3 coord);

	void set(Coord3 coord, voxel_type v);

	bool needs_calc_light = true;
	bool light_calculated = false;

	void calc_light_if_needed(DirectionalLight *light);
	void calc_light(DirectionalLight *light);

	// lighting
	bool get_light_raw(int i, int j, int k);
	bool has_light_raw(int i, int j, int k);

	int get_light(Coord3 coord);

	// meshing
	Mesh *mesh = 0;

	// terrian
	NoiseMap2 *height_map = 0;
	bool rasterized = false;
	int distance_from_player = 0;
};

