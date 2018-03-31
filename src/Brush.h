#pragma once
#include "types.h"
#include <unordered_map>

class Brush {
public:
	Brush() {
		update_profile();
	}

	std::unordered_map<Coord3, float> profile;

	int radius = 2;

	void update_profile() {
		for (int i = -radius; i <= radius; i++) {
			for (int j = -radius; j <= radius; j++) {
				for (int k = -radius; k <= radius; k++) {
					float dis = sqrt(i * i + j * j + k * k) / radius;

					if (dis > 1.0) {
						continue;
					}

					float value = pow(1 - dis, 2);

					Coord3 coord = { i,j,k };
					profile[coord] = value;
				}
			}
		}
	}
};