#include "gtest/gtest.h"
#include <Mesher.h>

//TEST(Mesher, MeshCube) {
//	Chunk *chunk = new Chunk(32);
//	chunk->set({ 1, 1, 1 }, 1);
//	MeshResult *r = Mesher::mesh_chunk(chunk, 0);
//
//	ASSERT_EQ(r->num_triangles(), 12 * 3);
//	ASSERT_EQ(r->num_vertices(), 24);
//}
//
//TEST(Mesher, MeshBigCube) {
//	Chunk *chunk = new Chunk(32);
//	for (int i = 1; i < 31; i++) {
//		for (int j = 1; j < 31; j++) {
//			for (int k = 1; k < 31; k++) {
//				chunk->set({ i, j, k }, 1);
//			}
//		}
//	}
//	MeshResult *r = Mesher::mesh_chunk(chunk, 0);
//
//	ASSERT_EQ(r->num_triangles(), 3 * 12);
//	ASSERT_EQ(r->num_vertices(), 6 * 4);
//}
//
//TEST(Mesher, MeshCubeAtEdge) {
//	Chunk *chunk = new Chunk(32);
//	chunk->set({ 0, 0, 0 }, 1);
//	MeshResult *r = Mesher::mesh_chunk(chunk, 0);
//
//	ASSERT_EQ(r->num_triangles(), 3 * 12);
//	ASSERT_EQ(r->num_vertices(), 6 * 4);
//}
//
//TEST(Mesher, MaskOneSquare) {
//	Mask *mask = new Mask(32);
//	MaskValue v = MaskValue(1, { 0, 0, 0, 0 });
//	mask->set(0, 0, v);
//	auto quads = mask->get_quads();
//
//	ASSERT_EQ(quads.size(), 1);
//	ASSERT_EQ(quads[0].x, 0);
//	ASSERT_EQ(quads[0].y, 0);
//	ASSERT_EQ(quads[0].w, 1);
//	ASSERT_EQ(quads[0].h, 1);
//}
//
//TEST(Mesher, MaskLine) {
//	Mask *mask = new Mask(32);
//	mask->set(0, 1, MaskValue(1));
//	mask->set(0, 2, MaskValue(1));
//	mask->set(0, 3, MaskValue(1));
//	auto quads = mask->get_quads();
//
//	ASSERT_EQ(quads.size(), 1);
//	ASSERT_EQ(quads[0].x, 0);
//	ASSERT_EQ(quads[0].y, 1);
//	ASSERT_EQ(quads[0].w, 1);
//	ASSERT_EQ(quads[0].h, 3);
//}
//
//TEST(Mesher, MaskMultiple) {
//	Mask *mask = new Mask(32);
//	mask->set(0, 1, MaskValue(1));
//	mask->set(2, 2, MaskValue(1));
//	mask->set(3, 3, MaskValue(1));
//	mask->set(3, 4, MaskValue(1));
//	mask->set(5, 5, MaskValue(1));
//	auto quads = mask->get_quads();
//
//	ASSERT_EQ(quads.size(), 4);
//}
//
//TEST(Mesher, MaskQuad) {
//	Mask *mask = new Mask(32);
//	for (int i = 1; i < 31; i++) {
//		for (int j = 1; j < 31; j++) {
//			MaskValue v = MaskValue(1);
//			mask->set(i, j, v);
//			ASSERT_FALSE(v.has_ao());
//		}
//	}
//
//	auto quads = mask->get_quads();
//
//	ASSERT_EQ(quads.size(), 1);
//	ASSERT_EQ(quads[0].x, 1);
//	ASSERT_EQ(quads[0].y, 1);
//	ASSERT_EQ(quads[0].w, 30);
//	ASSERT_EQ(quads[0].h, 30);
//}
//
//TEST(Mesher, MaskAO) {
//	Mask *mask = new Mask(32);
//	for (int i = 1; i < 31; i++) {
//		for (int j = 1; j < 31; j++) {
//			mask->set(i, j, MaskValue(1));
//		}
//	}
//
//	mask->set(1, 1, MaskValue(1, { 1, 2, 3, 4 }));
//
//	auto quads = mask->get_quads();
//
//	ASSERT_EQ(quads.size(), 3);
//}
//
//TEST(Mesher, MaskValueHasAO) {
//	MaskValue *value = new MaskValue(1);
//	ASSERT_FALSE(value->has_ao());
//}
//
//TEST(Mesher, MaskValueHasAO2) {
//	MaskValue *value = new MaskValue(1, { 0,0,0,0 });
//	ASSERT_FALSE(value->has_ao());
//}

//TEST(Mesher, CubeHasNoAO) {
//	Chunk *chunk = new Chunk();
//	chunk->set({ 1,2,3 }, 4);
//	MeshResult *result = Mesher::mesh_chunk(chunk, 0);
//	for (auto v : result->ao) {
//		ASSERT_EQ(v, 0);
//	}
//}