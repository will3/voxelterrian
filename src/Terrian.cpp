#include "Terrian.h"
#include "types.h"
#include "Mesher.h"
#include <algorithm>
#include <thread>
#include <mutex>
#include "Mesh.h"
#include "Brush.h"
#include "Field3.h"

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
				if (!chunk->rasterized) {
					rasterize_height(chunk);
					chunk->rasterized = true;
				}
			}
		}
	}

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
	player_position = camera_control->camera->target;
}

void Terrian::remove() {
}

void Terrian::set_draw_dis(int dis)
{
	smooth_light_dis = gen_geometry_dis = dis;
	calc_light_dis = dis + 1;
	tessellate_dis = remove_chunk_dis = dis + 2;
}

void Terrian::set_needs_rasterize() {
	for (auto coord : chunks->get_coords()) {
		Chunk *chunk = chunks->get_chunk(coord);
		if (chunk == 0) {
			continue;
		}
		chunk->rasterized = false;
	}
}

Terrian::Terrian()
{
	this->chunks = new Chunks();

	height_noise->amplitude = 128.0;
	height_noise->get_noise()->SetFractalOctaves(5);
	height_noise->y_scale = 0.4;

	canyon_noise->get_noise()->SetFractalType(FastNoise::RigidMulti);
	canyon_noise->get_noise()->SetFractalOctaves(1);

	for (auto mark : rock_color_gradient->getMarks()) {
		rock_color_gradient->removeMark(mark);
	}

	rock_color_gradient->addMark(0.0, ImColor(255, 255, 255));
	rock_color_gradient->addMark(1.0, ImColor(255, 255, 255));
}

Terrian::~Terrian()
{
	delete height_noise;
}

void Terrian::rasterize_height(Chunk * chunk) {
	Coord3 offset = chunk->get_offset();

	int noise_size = 17;
	Field3<float> field = Field3<float>(noise_size);
	//Field3<float> canyon_field = Field3<float>(noise_size);

	for (int i = 0; i < noise_size; i++) {
		for (int j = 0; j < noise_size; j++) {
			for (int k = 0; k < noise_size; k++) {
				Coord3 coord = Coord3(i, j, k) * 2 + offset;

				float j_offset = overhang_noise->get_noise()->GetSimplexFractal(coord.i, coord.j, coord.k);
				float fractal = height_noise->get_noise()->GetSimplexFractal(coord.i + j_offset, coord.j * height_noise->y_scale, coord.k + j_offset);
				float density = fractal * height_noise->amplitude - coord.j;

				//if (density > 1.0) {
				//	density = 1.0;
				//}

				//float canyon = canyon_noise->get_noise()->GetSimplexFractal(coord.i, coord.j * 0.0, coord.k);

				//canyon = (canyon + 1) * 0.5;

				//canyon_field.set(i, j, k, canyon);

				//float ratio = 1 - ((canyon - 0.8) / 0.2);

				//density *= ratio;

		/*		float ratio = 1 - (canyon - 0.8) / (1 - 0.8);
				if (ratio > 1.0) {
					ratio = 1.0;
				}

				if (ratio < 0.0) {
					ratio = 0.0;
				}*/

				//float density = height_noise->noise->GetSimplexFractal(coord.i, coord.j * 0.4, coord.k) * 128.0 - coord.j;
				field.set(i, j, k, density);
			}
		}
	}

	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				Coord3 chunk_coord = Coord3(i, j, k) + offset;
	/*			float canyon = canyon_field.sample(i * 0.5, j * 0.5, k * 0.5);

				if (canyon > 0.8) {
					continue;
				}
*/
				float density = field.sample(i * 0.5, j * 0.5, k * 0.5);

				if (density > 0.5) {
					chunk->set({ i, j, k }, density > 0.5 ? 1 : 0);
					std::array<float, 3>color;
					rock_color_gradient->getColorAt(chunk_coord.j / max_height, color.data());
					chunk->set_color({ i,j,k }, UINT8_C(color[0] * 255), UINT8_C(color[1] * 255), UINT8_C(color[2] * 255));
				}

				/*chunk->set({ i, j, k }, density > 0.5 ? 1 : 0);

				if (density > 0.5 ) {
					if (canyon > 0.8) {
						chunk->set_color({ i,j,k }, UINT8_C(255), UINT8_C(0), UINT8_C(0));
					}
					else {
						chunk->set_color({ i,j,k }, UINT8_C(255), UINT8_C(255), UINT8_C(255));
					}
				}*/
			}
		}
	}
}