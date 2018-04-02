#pragma once

#include <vector>
#include "Node.h"
#include "Geometry.h"
#include "Material.h"

class Mesh : public Node {
private:
	Geometry * geometry;
	Material * material;
public:
	Mesh(Geometry *geometry, Material *material);

	Geometry *get_geometry();
	Material *get_material();

	void render(Camera *camera);

	~Mesh();
};