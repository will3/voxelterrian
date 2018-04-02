#pragma once;
#include "Geometry.h"
#include <array>

struct VoxelVertice {
	std::array<GLint, 3> position;
	std::array<GLint, 3> color;
	GLint lighting = max_light_value;
};

class VoxelGeometry : public Geometry {
private:
	ArrayBuffer<GLint, GL_INT> positions = ArrayBuffer<GLint, GL_INT>(0, 3);
	ArrayBuffer<GLint, GL_INT> colors = ArrayBuffer<GLint, GL_INT>(1, 3);
	ArrayBuffer<GLint, GL_INT> lighting = ArrayBuffer<GLint, GL_INT>(2, 1);
	ElementBuffer indices;
public:
	void push_vertice(float x, float y, float z, int r, int g, int b, int light) {
		positions.get_data().push_back(x);
		positions.get_data().push_back(y);
		positions.get_data().push_back(z);

		colors.get_data().push_back(r);
		colors.get_data().push_back(g);
		colors.get_data().push_back(b);

		lighting.get_data().push_back(light);
	}

	ArrayBuffer<GLint, GL_INT>& get_positions() {
		return positions;
	}

	ArrayBuffer<GLint, GL_INT>& get_colors() {
		return colors;
	}

	ArrayBuffer<GLint, GL_INT>& get_lighting() {
		return lighting;
	}

	ElementBuffer& get_indices() {
		return indices;
	}

	void load() {
		positions.load();
		colors.load();
		lighting.load();
		indices.load();
	}

	void unload() {
		positions.unload();
		colors.unload();
		lighting.unload();
		indices.unload();
	}

	void bind()
	{
		positions.bind();
		colors.bind();
		lighting.bind();
		indices.bind();
	}

	void unbind()
	{
		positions.unbind();
		colors.unbind();
		lighting.unbind();
		indices.unbind();
	}

	int get_indices_count() {
		return indices.get_data().size();
	}
};