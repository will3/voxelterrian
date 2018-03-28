#pragma once

#include "Chunks.h"
#include <unordered_map>
#include "FastNoise.h"
#include "Entity.h"
#include "types.h"
#include "NoiseMap2.h"
#include "DirectionalLight.h"

class MeshResult;

class Terrian : public Entity {
public:
	int max_height = 128;
	int max_chunks_y = 8;

	int rasterize_dis = 7;
	int generate_masks_dis = 6;
	int chunking_dis = 6;
	int discard_dis = 8;

	std::vector<Coord3> coords_within_dis(int dis);
	void rasterize_height_chunk(Chunk *chunk);

	FastNoise *height_noise = new FastNoise();

	Terrian();
	~Terrian();

	void start();
	void update();
	void remove();

	DirectionalLight *light;
	Chunks *chunks;
	//ISceneManager* scene;
	//irr::s32 voxelMaterial;
	float3 player_position = float3(0, 0, 200);
	int chunk_size = 32;
	Coord2 player_origin;
};
