#pragma once
#include "Entity.h"
#include "Scene.h"
#include "Mesh.h"

class Test : public Entity {
private:
	Mesh * mesh = 0;

public:
	Scene *scene;

	Mesh* get_mesh() {
		if (mesh == 0) {
			Chunks *chunks = new Chunks();
			chunks->set({ 0,0,0 }, 1);
			Chunk *chunk = chunks->get_chunk({ 0,0,0 });
			DirectionalLight *light = new DirectionalLight();
			Mesher::gen_masks(chunk, chunks, light);
			Mesher::gen_geometry(chunk);

			Material *material = new Material();
			mesh = new Mesh(chunk->geometry, material);
		}
		return mesh;
	}

	void start() {
		scene->add(get_mesh());
	}
};

//Geometry *geometry = new Geometry();
//geometry->vertices.insert(geometry->vertices.end(), {
//	-1, -1, 0,
//	1, -1, 0,
//	0,  1, 0
//	});
//geometry->colors.insert(geometry->colors.end(), {
//	255,0,0,
//	0,255,0,
//	0,0,255
//	});
//geometry->lighting.insert(geometry->lighting.end(), {
//	15, 15, 15
//	});
//geometry->indices.insert(geometry->indices.end(), {
//	0,1,2
//	});