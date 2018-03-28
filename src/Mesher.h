#pragma once
#include "Chunk.h"
#include "Chunks.h"
#include "Mask.h"
#include "DirectionalLight.h"

class Mesher {
private:
	static Voxel get_voxel(Coord3 &coord, Chunk &chunk, Chunks &chunks);
	static Coord3 get_vertice(int i, int j, int k, int d);
	static ao_type get_ao(int s1, int s2, int c);
	//static void copy_quad(Mask& mask, bool front, int i, int d, MeshResult *buffer);
public:
	static void generate_masks(Chunk* chunk, Chunks* chunks, DirectionalLight *light);
	//static void copy_mask(Mask * mask, SMeshBuffer * buffer);
};