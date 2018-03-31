#include "Terrian.h"
#include "types.h"
#include "Mesher.h"
#include <algorithm>
#include <thread>
#include <mutex>
#include "Mesh.h"
#include "Brush.h"

inline int fast_floor(int x, int y) {
	return x / y - (x % y < 0);
}

static Brush *shadow_brush = new Brush();

void terrian_work(Terrian *terrian) {
	Chunks *chunks = terrian->chunks;
	DirectionalLight *light = terrian->light;
	glm::vec3 player_position = terrian->player_position;
	Coord2 player_origin = terrian->player_origin;
	int chunk_size = terrian->chunk_size;

	// Tessallate
	for (auto origin : terrian->coords_within_dis(terrian->tessellate_dis)) {
		Chunk *chunk = chunks->get_or_create_chunk(origin);
		if (!chunk->rasterized) {
			terrian->rasterize_height_chunk(chunk);
			chunk->rasterized = true;
		}
	}

	// Update distance from player
	for (auto origin : chunks->get_coords()) {
		Chunk *chunk = chunks->get_or_create_chunk(origin);
		chunk->distance_from_player = std::max(abs(player_origin.i - origin.i), abs(player_origin.j - origin.k));
	}

	// Calc light
	for (auto coord : terrian->coords_within_dis(terrian->calc_light_dis)) {
		Chunk *chunk = chunks->get_chunk(coord);
		chunk->calc_light_if_needed(terrian->light);
	}

	// Smooth light
	for (auto coord : terrian->coords_within_dis(terrian->smooth_light_dis)) {
		Chunk *chunk = chunks->get_chunk(coord);
		chunk->smooth_light_if_needed(terrian->light, shadow_brush);
	}

	// Mesh
	for (auto coord : terrian->coords_within_dis(terrian->gen_geometry_dis)) {
		Chunk *chunk = chunks->get_chunk(coord);
		if (!chunk->dirty) {
			continue;
		}

		if (chunk->mesh != 0) {
			chunk->mesh->get_geometry()->drop();
			chunk->mesh->parent->remove(chunk->mesh);
		}

		Geometry *geometry = Mesher::mesh(chunk, chunks);
		Mesh *mesh = new Mesh(geometry, terrian->material);
		mesh->position = chunk->position;
		terrian->scene->add(mesh);
		chunk->mesh = mesh;

		chunk->dirty = false;
	}

	// Discard
	for (auto coord : chunks->get_coords()) {
		Chunk *chunk = chunks->get_chunk(coord);
		if (chunk->distance_from_player > terrian->remove_chunk_dis) {
			if (chunk->mesh != 0) {
				chunk->mesh->get_geometry()->drop();
				chunk->mesh->remove_self();
			}
			chunks->delete_chunk(chunk->get_origin());
		}
	}
}

void terrian_worker(Terrian *terrian) {
	while (!terrian->removed) {
		terrian_work(terrian);
	}
}

std::vector<Coord3> Terrian::coords_within_dis(int dis) {
	Coord2 start = player_origin - Coord2(dis, dis);
	Coord2 end = player_origin + Coord2(dis, dis);

	std::vector<Coord3> coords;

	// rasterize height
	for (int i = start.i; i <= end.i; i++) {
		for (int j = 0; j < max_chunks_y; j++) {
			for (int k = start.j; k <= end.j; k++) {
				coords.push_back({ i, j, k });
			}
		}
	}

	return coords;
}

void Terrian::start() {
	player_origin = { fast_floor(player_position.x, chunk_size), fast_floor(player_position.z, chunk_size) };
	std::thread t(terrian_worker, this);
	t.detach();
}

void Terrian::update() {
	player_origin = { fast_floor(player_position.x, chunk_size), fast_floor(player_position.z, chunk_size) };
}

void Terrian::remove() {
}

void Terrian::set_draw_dis(int dis)
{
	smooth_light_dis = gen_geometry_dis = dis;
	calc_light_dis = dis + 1;
	tessellate_dis = remove_chunk_dis = dis + 2;
}

void Terrian::rasterize_height_chunk(Chunk *chunk) {
	Coord3 offset = chunk->get_offset();

	if (chunk->height_map == 0) {
		chunk->height_map = new NoiseMap2(height_noise, chunk->size, offset.ik(), 1);
		chunk->height_map->amplitude = max_height;
		chunk->height_map->populate();
	}

	for (int i = 0; i < chunk_size; i++) {
		for (int k = 0; k < chunk_size; k++) {
			Coord2 uv = { i,k };

			float height = chunk->height_map->sample(uv);

			if (height < offset.j) {
				continue;
			}

			for (int j = 0; j < fmin(chunk_size, height - offset.j); j++) {
				chunk->set({ i, j, k }, 1);
			}
		}
	}
}

Terrian::Terrian()
{
	this->chunks = new Chunks(chunk_size);
}

Terrian::~Terrian()
{
	delete height_noise;
}