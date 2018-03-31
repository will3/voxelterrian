#pragma once
#include "Chunk.h"
#include "Chunks.h"
#include "Mask.h"
#include "DirectionalLight.h"
#include "Mesh.h"
#include "Geometry.h"

class Mesher {
private:
	static voxel_type get_voxel(Coord3 &coord, Chunk &chunk, Chunks &chunks);
	static Coord3 get_vertice(int i, int j, int k, int d);
	static ao_type get_ao(int s1, int s2, int c);
	static bool stop_merge(MaskValue& c, MaskValue& next);
	static void copy_quads(Mask& mask, Geometry *geometry, int x, int y, int w, int h, int ao0, int ao1, int ao2, int ao3, int l);
	static void copy_quads(Mask& mask, Geometry *geometry);

public:
	static Geometry* mesh(Chunk* chunk, Chunks* chunks);
};