#pragma once

#include "FastNoise.h"
#include "types.h"
#include "Entity.h"
#include "MarchingCube.h"
#include "StandardGeometry.h"
#include "StandardMaterial.h"
#include "Mesh.h"
#include "Scene.h"
#include "VoxelGeometry.h"
#include "VoxelMaterial.h"
#include "Field3.h"
#include <glm/gtc/quaternion.hpp>

class Terrian2 : public Entity {
private:
	float draw_distance = 64.0;
	float draw_height = 128.0;
	float height_noise_y_scale = 0.4;
	float height_noise_amplitude = 128.0;
	FastNoise *height_noise = new FastNoise();
	StandardMaterial *material = new StandardMaterial();
	Chunks *chunks = new Chunks();
public:
	Scene *scene;

	Terrian2() {
		height_noise->SetFractalOctaves(5);
		//canyon_noise->get_noise()->SetFractalType(FastNoise::RigidMulti);
		//canyon_noise->get_noise()->SetFractalOctaves(1);
	}

	void start() {
		int num_chunks = ceil(draw_distance / (float)CHUNK_SIZE);
		int num_chunks_y = ceil(draw_height / (float)CHUNK_SIZE);
		Coord3 view_origin = { 0, 0, 0 };

		for (int i = -num_chunks; i <= num_chunks; i++) {
			for (int k = -num_chunks; k <= num_chunks; k++) {
				for (int j = 0; j < num_chunks_y; j++) {
					Coord3 origin = view_origin + Coord3(i, j, k);
					draw(origin);
				}
			}
		}


	}

	void update() {
	}

	float get_density(float x, float y, float z) {
		float fractal = height_noise->GetSimplexFractal(x, y * height_noise_y_scale, z);
		float density = fractal * height_noise_amplitude - y;
		return density;
	}

	void draw(Coord3 origin) {
		StandardGeometry *geometry = new StandardGeometry();
		Chunk *chunk = chunks->get_or_create_chunk(origin);
		Coord3 offset = chunk->get_offset();

		int noise_size = 17;
		Field3<float> field = Field3<float>(noise_size);

		for (int i = 0; i < noise_size; i++) {
			for (int j = 0; j < noise_size; j++) {
				for (int k = 0; k < noise_size; k++) {
					Coord3 coord = Coord3(i, j, k) * 2 + offset;

					float fractal = height_noise->GetSimplexFractal(coord.i, coord.j * height_noise_y_scale, coord.k);
					float density = fractal * height_noise_amplitude - coord.j;
					field.set(i, j, k, density);
				}
			}
		}

		for (int i = 0; i < CHUNK_SIZE; i++) {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				for (int k = 0; k < CHUNK_SIZE; k++) {
					Coord3 chunk_coord = Coord3(i, j, k) + offset;
					float density = field.sample(i * 0.5, j * 0.5, k * 0.5);

					if (density > 0.5) {
						chunk->set({ i, j, k }, density > 0.5 ? 1 : 0);
					}
				}
			}
		}

		std::vector<Vertex> vertices;
		std::vector<int> indicesOut;

		auto getDensity = [=](float x, float y, float z) {
			return get_density(x, y, z);
		};

		//auto getDensity = std::bind(&Terrian2::get_density, this, _1, _2, _3);
		glm::vec3 offsetVec = glm::vec3(offset.i, offset.j, offset.k);
		generate_geometry(getDensity, offsetVec, CHUNK_SIZE, vertices, indicesOut);

		auto& indices = geometry->get_indices();

		for (auto vertex : vertices) {
			glm::vec3 position = { vertex.position[0], vertex.position[1], vertex.position[2] };
			glm::vec3 normal = { vertex.normal[0], vertex.normal[1], vertex.normal[2] };
			geometry->push_vertice(position, normal, { 255,255,255 });
		}

		for (auto indice : indicesOut) {
			indices.push_back(indice);
		}

		Mesh *mesh = new Mesh(geometry, material);
		mesh->position = { offset.i, offset.j, offset.k };

		scene->add(mesh);
	}

	~Terrian2() {
		delete height_noise;
		//delete canyon_noise;
	}
};