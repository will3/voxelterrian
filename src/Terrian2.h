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

using namespace glm;

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
	ImGradient *rock_color_gradient = new ImGradient();

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

	ivec3 get_color(glm::vec3 coord) {
		return get_color(coord.x, coord.y, coord.z);
	}

	ivec3 get_color(float x, float y, float z) {
		float position = y / height_noise_amplitude;
		std::array<float, 3> color;
		rock_color_gradient->getColorAt(position, color.data());
		ivec3 c = { color[0] * 255, color[1] * 255, color[2] * 255 };
		return c;
	}

	void draw(Coord3 origin) {
		StandardGeometry *geometry = new StandardGeometry();
		float chunk_size = 32;
		Coord3 offset = origin * chunk_size;
		vec3 offsetV = { offset.i, offset.j, offset.k };

		float noise_size = chunk_size / 2 + 10;
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

		std::vector<Vertex> vertices;
		std::vector<int> indicesOut;

		//auto getDensity = std::bind(&Terrian2::get_density, this, _1, _2, _3);
		glm::vec3 offsetVec = glm::vec3(offset.i, offset.j, offset.k);
		generate_geometry(offsetVec, CHUNK_SIZE, vertices, indicesOut, field, 2);

		auto& indices = geometry->get_indices();

		for (auto vertex : vertices) {
			glm::vec3 position = { vertex.position[0], vertex.position[1], vertex.position[2] };
			glm::vec3 normal = { vertex.normal[0], vertex.normal[1], vertex.normal[2] };
			ivec3 color = get_color(position + offsetV);
			geometry->push_vertice(position, normal, { color.x, color.y, color.z });
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