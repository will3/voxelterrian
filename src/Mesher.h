#pragma once
#include "Chunk.h"
#include "Chunks.h"
#include "Mask.h"
#include "DirectionalLight.h"
#include "Mesh.h"

class Mesher {
private:
	static Voxel get_voxel(Coord3 &coord, Chunk &chunk, Chunks &chunks);
	static Coord3 get_vertice(int i, int j, int k, int d);
	static ao_type get_ao(int s1, int s2, int c);
	static bool stop_merge(MaskValue& c, MaskValue& next);
	static void gen_geometry(Mask *mask, Geometry *geometry, int x, int y, int w, int h, int ao0, int ao1, int ao2, int ao3, int l);
	static void gen_geometry(Mask* mask, Geometry *geometry);

public:
	static void gen_masks(Chunk* chunk, Chunks* chunks, DirectionalLight *light);
	static void gen_geometry(Chunk* chunk);
};