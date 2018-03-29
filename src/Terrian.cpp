#include "Terrian.h"
#include "types.h"
#include "Logger.h"
#include "Mesher.h"
#include <algorithm>
#include <thread>
#include <mutex>
#include "Mesh.h"

inline int fast_floor(int x, int y) {
	return x / y - (x % y < 0);
}

class UpdateDistanceFromPlayer: public ChunksVisitor {
public:
	UpdateDistanceFromPlayer(Coord2 player_origin) : player_origin(player_origin) {}
	Coord2 player_origin;
	void visit(Chunk *chunk) {
		Coord3 origin = chunk->get_origin();
		chunk->distance_from_player = std::max(abs(player_origin.i - origin.i), abs(player_origin.j - origin.k));
	}
};

class GenerateMasks : public ChunksVisitor {
private:
	Terrian * terrian;
	Chunks *chunks;
	DirectionalLight *light;
public:
	GenerateMasks(Terrian *terrian, Chunks *chunks, DirectionalLight *light) : terrian(terrian), chunks(chunks), light(light) {};
	void visit(Chunk *chunk) {
		int chunk_size = terrian->chunk_size;
		if (chunk->distance_from_player > terrian->generate_masks_dis) {
			return;
		}
		if (!chunk->dirty) {
			return;
		}
		Mesher::generate_masks(chunk, chunks, light);
		chunk->dirty = false;
	}
};

class MeshChunk : public ChunksVisitor {
public:
	void visit(Chunk *chunk) {
		int chunk_size = chunk->size;
		if (chunk->masks.size() > 0 && chunk->mesh == 0) {
			chunk->mesh = new Mesh();
			Mesher::insert_quads(chunk);

			for (Mask *mask : chunk->masks) {
				delete mask;
			}
			chunk->masks.clear();
		}
	}
};

class DiscardChunk : public ChunksVisitor {
public:
	DiscardChunk(Chunks *chunks, int discard_dis) :chunks(chunks), discard_dis(discard_dis) {};
private:
	int discard_dis;
	Chunks *chunks;

	void visit(Chunk *chunk) {
		if (chunk->distance_from_player > discard_dis) {
			if (!chunks->remove_chunk(chunk->get_origin())) {
				throw "chunk not released!";
			}
		}
	}
};

class AddNodeToScene : public ChunksVisitor {
private:
	//ISceneManager * scene;
	//s32 voxelMaterial;

public:
	/*AddNodeToScene(ISceneManager *scene, s32 voxelMaterial) : scene(scene), voxelMaterial(voxelMaterial) {};
	void visit(Chunk *chunk) {
		if (chunk->mesh != 0 && chunk->node == 0) {
			chunk->drop_node_if_needed();
			IMeshSceneNode* node = scene->addMeshSceneNode(chunk->mesh);
			node->setPosition(chunk->position);
			node->setMaterialType((video::E_MATERIAL_TYPE)voxelMaterial);
			chunk->node = node;
			chunk->mesh = 0;
		}
	}*/
};

void terrian_work(Terrian *terrian) {
	Chunks *chunks = terrian->chunks;
	DirectionalLight *light = terrian->light;
	float3 player_position = terrian->player_position;
	Coord2 player_origin = terrian->player_origin;
	int chunk_size = terrian->chunk_size;

	// rasterize
	auto coords_within_dis = terrian->coords_within_dis(terrian->rasterize_dis);
	for (auto origin : coords_within_dis) {
		Chunk *chunk = chunks->get_or_create_chunk(origin);
		if (chunk->rasterized) {
			continue;
		}
		terrian->rasterize_height_chunk(chunk);
		chunk->rasterized = true;
	}

	UpdateDistanceFromPlayer update_distance = UpdateDistanceFromPlayer(player_origin);
	chunks->visit_chunks(update_distance);

	GenerateMasks generate_masks = GenerateMasks(terrian, chunks, light);
	chunks->visit_chunks(generate_masks);

	MeshChunk mesh_chunks = MeshChunk();
	chunks->visit_chunks(mesh_chunks);
}

void terrian_worker(Terrian *terrian) {
	while (!terrian->removed) {
		terrian_work(terrian);
	}
}

std::vector<Coord3> Terrian::coords_within_dis(int dis) {
	Coord2 start = player_origin - Coord2(rasterize_dis, rasterize_dis);
	Coord2 end = player_origin + Coord2(rasterize_dis, rasterize_dis);

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

	DiscardChunk discardChunk = DiscardChunk(chunks, discard_dis);
	chunks->visit_chunks(discardChunk);

	//AddNodeToScene addToScene = AddNodeToScene(scene, voxelMaterial);
	//chunks->visit_chunks(addToScene);
}

void Terrian::remove() {
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