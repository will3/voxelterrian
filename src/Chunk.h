#pragma once
#include <vector>
#include "types.h"
#include "Mask.h"
#include "NoiseMap2.h"
#include "Mesh.h"
#include <unordered_map>

typedef GLbyte voxel_type;
class DirectionalLight;
class Chunks;
class Brush;

class Chunk
{
private:
	std::vector<voxel_type> data;
	Coord3 origin;
	Coord3 offset;

	std::unordered_map<int, bool> raw_light_map;
	std::unordered_map<int, int> smooth_light_map;

public:
	Chunk(int size, Coord3 origin);
	Chunk(int size = 32);
	~Chunk();
	Coord3& get_origin() { return origin; }
	Coord3& get_offset() { return offset; }
	int size;
	bool dirty;
	Chunks *chunks;

	bool shadow_softened = false;
	glm::vec3 position;

	voxel_type get(Coord3 coord);
	voxel_type get_global(Coord3 coord);

	void set(Coord3 coord, voxel_type v);

	bool needs_calc_light = true;
	bool needs_smooth_light = true;
	bool light_calculated = false;

	void calc_light_if_needed(DirectionalLight *light);
	void calc_light(DirectionalLight *light);

	void smooth_light_if_needed(DirectionalLight *light, Brush *brush);
	void smooth_light(DirectionalLight *light, Brush *brush);

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

