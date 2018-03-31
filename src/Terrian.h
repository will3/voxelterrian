#pragma once

#include "Chunks.h"
#include <unordered_map>
#include "FastNoise.h"
#include "Entity.h"
#include "types.h"
#include "NoiseMap2.h"
#include "DirectionalLight.h"
#include "Scene.h"

class MeshResult;

class Terrian : public Entity {
public:
	int max_height = 128;
	int max_chunks_y = 8;

	int tessellate_dis = 7;
	int calc_light_dis = 6;
	int smooth_light_dis = 5;
	int gen_geometry_dis = 5;
	int remove_chunk_dis = 7;

	std::vector<Coord3> coords_within_dis(int dis);
	void rasterize_height_chunk(Chunk *chunk);

	FastNoise *height_noise = new FastNoise();

	Terrian();
	~Terrian();

	void start();
	void update();
	void remove();
	void set_draw_dis(int dis);

	DirectionalLight *light;
	Chunks *chunks;
	Scene *scene;
	Material *material;
	glm::vec3 player_position = glm::vec3(0, 0, 0);
	int chunk_size = 32;
	Coord2 player_origin;
};

void terrian_work(Terrian * terrian);
