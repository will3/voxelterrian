#pragma once

#include "Chunks.h"
#include <unordered_map>
#include "FastNoise.h"
#include "Entity.h"
#include "types.h"
#include "NoiseMap2.h"
#include "DirectionalLight.h"
#include "Scene.h"
#include "Noise.h"

#define max_height 128

class MeshResult;

class Terrian : public Entity {
private:
	std::vector<Coord3> coords_within_dis(int dis);

public:
	int max_chunks_y = 4;
	int tessellate_dis = 7;
	int calc_light_dis = 6;
	int smooth_light_dis = 5;
	int gen_geometry_dis = 5;
	int remove_chunk_dis = 7;
	bool dirty = false;

	Noise *height_noise = new Noise();
	DirectionalLight *light;
	Chunks *chunks;
	Scene *scene;
	Material *material;
	Coord2 player_origin;

	Terrian();
	~Terrian();

	void start();
	void update();
	void remove();
	void set_draw_dis(int dis);
	void update_terrian();
	glm::vec3 player_position = glm::vec3(0, 0, 0);
};