#pragma once
#include "Chunks.h"
#include <math.h> 
#include <array>
#include <Windows.h>
#include <string>
#include <GL/glew.h>

class RayCastResult {
public:
	int v;
	float3 hit_pos;
	float3 hit_norm;
};

class Raycast {
public:
	static RayCastResult raycast(Chunks *chunks, float3 position, float3 direction, int max_d, bool ignore_first, int max_y);
	static RayCastResult raycast(Chunks *chunks, float3 position, float3 direction, int max_d, bool ignore_first);
	static RayCastResult raycast(Chunks *chunks, float3 position, float3 direction, int max_d);
};