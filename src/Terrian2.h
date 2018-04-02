#pragma once

#include "FastNoise.h"
#include "Noise.h"
#include "Density_SSBO.h"
#include "types.h"
#include "Entity.h"
#include "ComputeShader.h"
#include "MarchingCube.h"
#include "StandardGeometry.h"
#include "StandardMaterial.h"
#include "Mesh.h"
#include "Scene.h"
#include "VoxelGeometry.h"
#include "VoxelMaterial.h"

class Terrian2 : public Entity {
public:
	Noise *height_noise = new Noise();
	Noise *canyon_noise = new Noise();
	StandardGeometry *geometry = new StandardGeometry();
	StandardMaterial *material = new StandardMaterial();
	Scene *scene;

	Terrian2() {
		height_noise->amplitude = 128.0;
		height_noise->get_noise()->SetFractalOctaves(5);
		height_noise->y_scale = 0.4;

		canyon_noise->get_noise()->SetFractalType(FastNoise::RigidMulti);
		canyon_noise->get_noise()->SetFractalOctaves(1);
	}

	void start() {
		draw(Coord3(0, 0, 0), 32);
	}

	void draw(Coord3 start_coord, int size) {
		Chunk *chunk = new Chunk();
		chunk->set({ 1, 1, 1 }, 1);
		std::vector<Vertex> vertices;
		std::vector<int> indicesOut;
		generate_geometry(chunk, vertices, indicesOut);

		auto& indices = geometry->get_indices().get_data();

		for (auto vertex : vertices) {
			glm::vec3 position = { vertex.position[0], vertex.position[1], vertex.position[2] };
			glm::vec3 normal = { vertex.normal[0], vertex.normal[1], vertex.normal[2] };
			geometry->push_vertice(position, normal, { 255,255,255 });
		}

		for (auto indice : indicesOut) {
			indices.push_back(indice);
		}

		Mesh *mesh = new Mesh(geometry, material);

		scene->add(mesh);
	}

	~Terrian2() {
		delete height_noise;
		delete canyon_noise;
	}
};