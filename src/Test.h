#pragma once
#include "Entity.h"
#include "Scene.h"
#include "Mesh.h"

class Test : public Entity {
public:
	Scene * scene;

	void start() {
		Geometry *geometry = new Geometry();
		geometry->vertices.insert(geometry->vertices.end(), {
			-1, -1, 0,
			1, -1, 0,
			0,  1, 0
			});
		geometry->colors.insert(geometry->colors.end(), {
			255,0,0,
			0,255,0,
			0,0,255
			});
		geometry->lighting.insert(geometry->lighting.end(), {
			15, 15, 15
			});
		geometry->indices.insert(geometry->indices.end(), {
			0,1,2
			});
		geometry->load_vbo();

		Material *material = new Material();

		Mesh *mesh = new Mesh(geometry, material);

		scene->add(mesh);
	}
};