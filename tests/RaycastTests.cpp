#include "gtest/gtest.h"
#include <Raycast.h>
#include <Windows.h>
#include <math.h>

void assert_array_eq(float3 a, float3 b);

TEST(VoxelRaycast, CheckHalt) {
	Chunks *chunks = new Chunks();
	float3 pos;
	float3 direction;

	for (int i = 0; i < 100000; ++i) {
		for (int j = 0; j < 3; ++j) {
			pos[j] = rand() * 10;
			direction[j] = rand() - 0.5;
		}
		Raycast::raycast(chunks, pos, direction, 100);
	}
}

TEST(VoxelRaycast, CheckHit) {
	Chunks *chunks = new Chunks();
	chunks->set({ 0, 0, 0 }, 1);

	RayCastResult result = Raycast::raycast(chunks, { -1, 0.5, 0.5 }, { 1, 0, 0 }, 100);
	ASSERT_EQ(result.v, 1);
	assert_array_eq(result.hit_pos, { 0, 0.5, 0.5 });
	assert_array_eq(result.hit_norm, { -1, 0, 0 });
}

TEST(VoxelRaycast, CheckHit2) {
	Chunks *chunks = new Chunks();
	chunks->set({ 0, 0, 0 }, 1);

	RayCastResult result = Raycast::raycast(chunks, { 2, 0.5, 0.5 }, { -1, 0, 0 }, 100);
	ASSERT_EQ(result.v, 1);
	assert_array_eq(result.hit_pos, { 1, 0.5, 0.5 });
	assert_array_eq(result.hit_norm, { 1, 0, 0 });
}

TEST(VoxelRaycast, CheckHit3) {
	Chunks *chunks = new Chunks();
	chunks->set({ 0, 0, 0 }, 1);

	RayCastResult result = Raycast::raycast(chunks, { 0.5, -1, 0.5 }, { 0, 1, 0 }, 100);
	ASSERT_EQ(result.v, 1);
	assert_array_eq(result.hit_pos, { 0.5, 0, 0.5 });
	assert_array_eq(result.hit_norm, { 0, -1, 0 });
}

TEST(VoxelRaycast, CheckHit4) {
	Chunks *chunks = new Chunks();
	chunks->set({ 0, 0, 0 }, 1);

	RayCastResult result = Raycast::raycast(chunks, { 0.5, 2, 0.5 }, { 0, -1, 0 }, 100);
	ASSERT_EQ(result.v, 1);
	assert_array_eq(result.hit_pos, { 0.5, 1, 0.5 });
	assert_array_eq(result.hit_norm, { 0, 1, 0 });
}

TEST(VoxelRaycast, CheckHit5) {
	Chunks *chunks = new Chunks();
	chunks->set({ 0, 0, 0 }, 1);

	RayCastResult result = Raycast::raycast(chunks, { 0.5, 0.5, -1 }, { 0, 0, 1 }, 100);
	ASSERT_EQ(result.v, 1);
	assert_array_eq(result.hit_pos, { 0.5, 0.5, 0 });
	assert_array_eq(result.hit_norm, { 0, 0, -1 });
}

TEST(VoxelRaycast, CheckHit6) {
	Chunks *chunks = new Chunks();
	chunks->set({ 0, 0, 0 }, 1);

	RayCastResult result = Raycast::raycast(chunks, { 0.5, 0.5, 2 }, { 0, 0, -1 }, 100);
	ASSERT_EQ(result.v, 1);
	assert_array_eq(result.hit_pos, { 0.5, 0.5, 1 });
	assert_array_eq(result.hit_norm, { 0, 0, 1 });
}

TEST(VoxelRaycast, CheckHit7) {
	Chunks *chunks = new Chunks();
	chunks->set({ 0, 0, 0 }, 1);

	for (float d = 0.0; d < 1.0; d += 0.1) {
		RayCastResult result = Raycast::raycast(chunks, { 0.5, 0.5, -1.0 }, { 0, 0, 1 }, d);
		ASSERT_EQ(result.v, 0);
	}
}

TEST(VoxelRaycast, CheckHit8) {
	Chunks *chunks = new Chunks();
	chunks->set({ 0, 0, 0 }, 1);

	RayCastResult result = Raycast::raycast(chunks, { 0.5, 0.5, -1.0 }, { 0, 0, 1 }, 1);
	ASSERT_EQ(result.v, 1);
}

TEST(VoxelRaycast, Shallow) {
	Chunks *chunks = new Chunks();
	chunks->set({ 0, 0, 0 }, 1);

	ASSERT_EQ(Raycast::raycast(chunks,
		{ 1.6014050509453097f, -0.05316262501184864f, -1.1969442782079132f },
		{ -0.8007025204726549f, 0.02658131750592432f, 0.5984721441039565f },
		5.0).v, 1);

	ASSERT_EQ(Raycast::raycast(chunks,
		{ -1.41733953858252f, 1.4110806611407838f, 0.25000001f },
		{ 0.70866977429126f, -0.7055403255703919f, 0 },
		5.0).v, 1);

	ASSERT_EQ(Raycast::raycast(chunks,
		{ 1, 1, 1 },
		{ 1, 1, -1 },
		5.0).v, 0);

	ASSERT_EQ(Raycast::raycast(chunks,
		{ 1.9788948495478698f, -0.06569435085032517f, -0.2822400161197344f },
		{ -0.9894474247739349f, 0.032847175425162585f, 0.1411200080598672f },
		5.0).v, 1);

	ASSERT_EQ(Raycast::raycast(chunks,
		{ 1, 1, 0 },
		{ -1, -1, -1 },
		5.0).v, 0);
}

TEST(VoxelRaycast, Sphere) {
	Chunks *chunks = new Chunks();
	chunks->set({ 0, 0, 0 }, 1);

	float pi = std::atan(1) * 4;;
	//Cast a bunch of rays around a sphere, make sure they hit box surface
	for (float theta = 0; theta < pi * 2.0; theta += 0.25) {
		for (float phi = 0; phi < pi; phi += 0.25) {
			float3 dir = { cos(theta) * cos(phi),
				sin(theta) * cos(phi),
				sin(phi) };
			float min = 0.001;
			for (float x = min; x < 1.0; x += 0.25) {
				for (float y = min; y < 1.0; y += 0.25) {
					for (float z = min; z < 1.0; z += 0.25) {
						float3 pos = { x - 2.0f * dir[0], y - 2.0f * dir[1], z - 2.0f * dir[2] };
						RayCastResult result = Raycast::raycast(chunks, pos, dir, 10);
						ASSERT_EQ(result.v, 1);
					}
				}
			}
		}
	}
}

void assert_array_eq(float3 a, float3 b) {
	ASSERT_EQ(a.x, b.x);
	ASSERT_EQ(a.y, b.y);
	ASSERT_EQ(a.z, b.z);
}