#include "NoiseMap2.h"

NoiseMap2::NoiseMap2(FastNoise *noise, int size, Coord2 offset, int scale) : noise(noise), size(size), offset(offset), scale(scale) {
	data.resize(size * size);
}

NoiseMap2::~NoiseMap2()
{
}

void NoiseMap2::set(int i, int j, float v) {
	int index = i * size + j;
	data[index] = v;
}

float NoiseMap2::get(int i, int j) {
	int index = i * size + j;
	return data[index];
}

float NoiseMap2::sample(Coord2 coord) {
	int index = coord.i * size + coord.j;
	return data[index];
}

void NoiseMap2::populate() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			float v = noise->GetSimplex(i + offset.i, j + offset.j) * amplitude;
			set(i, j, v);
		}
	}
	populated = true;
}
