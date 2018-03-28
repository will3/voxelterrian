#include "DirectionalLight.h"
#include "Raycast.h"
#include "Mask.h"

float3 unit_vector(float3 vector) {
	float dis = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	return vector / dis;
}
DirectionalLight::DirectionalLight() {
	direction = unit_vector({ 0.2f, 1.0f, 0.5f });
}

DirectionalLight::~DirectionalLight()
{
}

int DirectionalLight::calc_light(Chunk *chunk, Chunks *chunks, Coord3 coord) {
	Coord3 chunks_coord = coord + chunk->get_offset();
	float3 position = float3(chunks_coord.i + 0.5f, chunks_coord.j + 0.5f, chunks_coord.k + 0.5f);

	bool hit = Raycast::raycast(chunks, position, direction, 100, true).v > 0;

	if (hit) {
		return 0;
	}

	return max_light_value;
}

int DirectionalLight::calc_light(Chunk *chunk, Chunks *chunks, int d, int i, int j, int k, bool front) {
	// facing light
	bool no_light = direction[d] == 0 || (direction[d] > 0 != front);
	if (no_light) {
		return 0;
	}

	int u = (d + 1) % 3;
	int v = (d + 2) % 3;

	Coord3 chunks_coord = Coord3(i, j, k).rotate(d) + chunk->get_offset();
	float3 position = float3(chunks_coord.i, chunks_coord.j, chunks_coord.k);
	position[u] += 0.5f;
	position[v] += 0.5f;

	bool hit = Raycast::raycast(chunks, position, direction, 100, true).v > 0;

	if (hit) {
		return 0;
	}

	return max_light_value;
}