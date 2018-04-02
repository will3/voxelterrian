#pragma once;
#include "Geometry.h"

class VoxelGeometry : public Geometry {
private:
	ArrayBuffer<GLint, GL_INT> vertices = ArrayBuffer<GLint, GL_INT>(0, 3);
	ArrayBuffer<GLint, GL_INT> colors = ArrayBuffer<GLint, GL_INT>(1, 3);
	ArrayBuffer<GLint, GL_INT> lighting = ArrayBuffer<GLint, GL_INT>(2, 1);
	ElementBuffer indices;
public:

	ArrayBuffer<GLint, GL_INT>& get_vertices() {
		return vertices;
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
		vertices.load();
		colors.load();
		lighting.load();
		indices.load();
	}

	void unload() {
		vertices.unload();
		colors.unload();
		lighting.unload();
		indices.unload();
	}

	void bind()
	{
		vertices.bind();
		colors.bind();
		lighting.bind();
		indices.bind();
	}

	void unbind()
	{
		vertices.unbind();
		colors.unbind();
		lighting.unbind();
		indices.unbind();
	}

	int get_indices_count() {
		return indices.get_data().size();
	}
};