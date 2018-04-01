#include "Terrian.h"
#include "types.h"
#include "Mesher.h"
#include <algorithm>
#include <thread>
#include <mutex>
#include "Mesh.h"
#include "Brush.h"
#include "Sculptor.h"

// It's not safe to delete gl resources in other threads
class RemoveMeshWorker : public Worker {
public:
	Mesh *mesh;
	RemoveMeshWorker(Mesh *mesh) :mesh(mesh) {}
	void run() {
		delete mesh->get_geometry();
		mesh->remove_self();
		delete mesh;
	}
};

inline int fast_floor(int x, int y) {
	return x / y - (x % y < 0);
}

static Brush *shadow_brush = new Brush();

void terrian_worker(Terrian *terrian) {
	while (!terrian->removed) {
		terrian->update_terrian();
	}
}

void Terrian::update_terrian() {
	player_origin = { fast_floor(player_position.x, CHUNK_SIZE), fast_floor(player_position.z, CHUNK_SIZE) };

	// rasterize height
	for (int i = player_origin.i - tessellate_dis; i <= player_origin.i + tessellate_dis; i++) {
		for (int j = 0; j < max_chunks_y; j++) {
			for (int k = player_origin.j - tessellate_dis; k <= player_origin.j + tessellate_dis; k++) {
				Coord3 origin = Coord3(i, j, k);
				Chunk *chunk = chunks->get_or_create_chunk(origin);
				if (dirty || !chunk->rasterized) {
					Sculptor::rasterize_height(chunk, height_noise);
					chunk->rasterized = true;
				}
			}
		}
	}

	dirty = false;

	// Update distance from player
	for (auto origin : chunks->get_coords()) {
		Chunk *chunk = chunks->get_or_create_chunk(origin);
		chunk->distance_from_player = std::max(abs(player_origin.i - origin.i), abs(player_origin.j - origin.k));
	}

	// Calc light
	for (auto kv : chunks->map) {
		Chunk *chunk = kv.second;
		if (chunk->distance_from_player > calc_light_dis) {
			continue;
		}
		chunk->calc_light_if_needed(light);
	}

	// Mesh
	for (auto kv : chunks->map) {
		Chunk *chunk = kv.second;

		if (chunk->distance_from_player > gen_geometry_dis) {
			continue;
		}

		if (!chunk->dirty) {
			continue;
		}

		if (chunk->mesh != 0) {
			dispatcher->add(new RemoveMeshWorker(chunk->mesh));
		}

		Geometry *geometry = Mesher::mesh(chunk, chunks);
		chunk->next_geometry = geometry;

		Mesh *mesh = new Mesh(geometry, material);
		mesh->position = chunk->position;
		scene->add(mesh);
		chunk->mesh = mesh;

		chunk->dirty = false;
	}

	// Discard
	for (auto coord : chunks->get_coords()) {
		Chunk *chunk = chunks->get_chunk(coord);
		if (chunk->distance_from_player > remove_chunk_dis) {
			if (chunk->mesh != 0) {
				dispatcher->add(new RemoveMeshWorker(chunk->mesh));
			}
			chunks->delete_chunk(chunk->get_origin());
		}
	}
}

std::vector<Coord3> Terrian::coords_within_dis(int dis) {
	Coord2 start = player_origin - Coord2(dis, dis);
	Coord2 end = player_origin + Coord2(dis, dis);

	std::vector<Coord3> coords;

	// rasterize height
	for (int i = player_origin.i - dis; i <= player_origin.i + dis; i++) {
		for (int j = 0; j < max_chunks_y; j++) {
			for (int k = player_origin.j - dis; k <= player_origin.j + dis; k++) {
				coords.push_back({ i, j, k });
			}
		}
	}

	return coords;
}

void Terrian::start() {
	player_origin = { fast_floor(player_position.x, CHUNK_SIZE), fast_floor(player_position.z, CHUNK_SIZE) };
	std::thread t(terrian_worker, this);
	t.detach();
}

void Terrian::update() {
	
}

void Terrian::remove() {
}

void Terrian::set_draw_dis(int dis)
{
	smooth_light_dis = gen_geometry_dis = dis;
	calc_light_dis = dis + 1;
	tessellate_dis = remove_chunk_dis = dis + 2;
}

Terrian::Terrian()
{
	this->chunks = new Chunks();

	height_noise->amplitude = 128.0;
}

Terrian::~Terrian()
{
	delete height_noise;
}