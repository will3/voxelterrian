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
#include "Dispatcher.h"
#include "imgui_color_gradient.h"
#include "EditorCameraControl.h"

#define max_height 128.0

class MeshResult;

class Terrian : public Entity {
private:
	std::vector<Coord3> coords_within_dis(int dis);
	void rasterize_height(Chunk * chunk);

public:
	int max_chunks_y = 4;
	int tessellate_dis = 7;
	int calc_light_dis = 6;
	int smooth_light_dis = 5;
	int gen_geometry_dis = 5;
	int remove_chunk_dis = 7;

	Noise *height_noise = new Noise();
	Noise *overhang_noise = new Noise();
	Noise *canyon_noise = new Noise();
	ImGradient *rock_color_gradient = new ImGradient();

	DirectionalLight *light;
	Chunks *chunks;
	Scene *scene;
	Material *material;
	Coord2 player_origin;
	Dispatcher *dispatcher;
	EditorCameraControl *camera_control;

	Terrian();
	~Terrian();

	void start();
	void update();
	void remove();
	void set_draw_dis(int dis);
	void update_terrian();
	void set_needs_rasterize();

	glm::vec3 player_position = glm::vec3(0, 0, 0);
};