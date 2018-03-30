#include "Terrian.h"
#include "types.h"
#include "Mesher.h"
#include <algorithm>
#include <thread>
#include <mutex>
#include "Mesh.h"

inline int fast_floor(int x, int y) {
	return x / y - (x % y < 0);
}

void terrian_work(Terrian *terrian) {
	Chunks *chunks = terrian->chunks;
	DirectionalLight *light = terrian->light;
	glm::vec3 player_position = terrian->player_position;
	Coord2 player_origin = terrian->player_origin;
	int chunk_size = terrian->chunk_size;

	for (auto origin : terrian->coords_within_dis(terrian->tessellate_dis)) {
		chunks->get_or_create_chunk(origin);
	}

	// rasterize
	for (auto coord : chunks->get_coords()) {
		Chunk *chunk = chunks->get_chunk(coord);
		if (chunk->rasterized) {
			continue;
		}
		terrian->rasterize_height_chunk(chunk);
		chunk->rasterized = true;
	}

	// Update distance_from_player
	for (auto coord : chunks->get_coords()) {
		Chunk *chunk = chunks->get_chunk(coord);
		Coord3 origin = chunk->get_origin();
		chunk->distance_from_player = std::max(abs(player_origin.i - origin.i), abs(player_origin.j - origin.k));
	}

	// Generate mask
	for (auto coord : chunks->get_coords()) {
		Chunk *chunk = chunks->get_chunk(coord);
		if (chunk->distance_from_player > terrian->gen_masks_dis) {
			continue;
		}
		if (!chunk->dirty) {
			continue;
		}
		Mesher::gen_masks(chunk, chunks, light);
		chunk->dirty = false;
	}

	// Generate geometry
	for (auto coord : chunks->get_coords()) {
		Chunk *chunk = chunks->get_chunk(coord);

		if (chunk->distance_from_player > terrian->gen_geometry_dis) {
			continue;
		}

		int chunk_size = chunk->size;
		if (chunk->masks.size() > 0 && chunk->geometry == 0) {
			Mesher::gen_geometry(chunk);
			chunk->geometry_ready = true;
		}
	}

	for (auto coord : chunks->get_coords()) {
		Chunk *chunk = chunks->get_chunk(coord);
		if (chunk->distance_from_player > terrian->remove_chunk_dis) {
			chunks->remove_chunk(chunk->get_origin());
		}
	}

	for (auto coord : chunks->get_coords()) {
		Chunk *chunk = chunks->get_chunk(coord);

		if (chunk->geometry_ready) {
			if (chunk->mesh != 0) {
				chunk->mesh->parent->remove(chunk->mesh);
			}
			Mesh *mesh = new Mesh(chunk->geometry, terrian->material);
			chunk->mesh = mesh;
			mesh->position = chunk->position;
			terrian->scene->add(mesh);
			chunk->geometry = 0;

			chunk->geometry_ready = false;
		}
	}
}

void terrian_worker(Terrian *terrian) {
	while (!terrian->removed) {
		terrian_work(terrian);
	}
}

std::vector<Coord3> Terrian::coords_within_dis(int dis) {
	Coord2 start = player_origin - Coord2(tessellate_dis, tessellate_dis);
	Coord2 end = player_origin + Coord2(tessellate_dis, tessellate_dis);

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
	gen_geometry_dis = dis;
	gen_masks_dis = dis + 1;
	tessellate_dis = dis + 2;
	remove_chunk_dis = dis + 1;
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