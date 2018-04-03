#pragma once
#include "Geometry.h"

class StandardGeometry : public Geometry {
private:
	ArrayBuffer<GLfloat, GL_FLOAT> positions = ArrayBuffer<GLfloat, GL_FLOAT>(0, 3);
	ArrayBuffer<GLfloat, GL_FLOAT> normals = ArrayBuffer<GLfloat, GL_FLOAT>(1, 3);
	ArrayBuffer<GLint, GL_INT> colors = ArrayBuffer<GLint, GL_INT>(2, 3);
	ElementBuffer indices;

public:
	void push_vertice(glm::vec3 position, glm::vec3 normal, glm::ivec3 color) {
		positions.get_data().push_back(position[0]);
		positions.get_data().push_back(position[1]);
		positions.get_data().push_back(position[2]);

		normals.get_data().push_back(normal[0]);
		normals.get_data().push_back(normal[1]);
		normals.get_data().push_back(normal[2]);

		colors.get_data().push_back(color[0]);
		colors.get_data().push_back(color[1]);
		colors.get_data().push_back(color[2]);
	}

	unsigned int getIndex() {
		return positions.get_data().size() / 3;
	}

	std::vector<GLfloat>& get_positions() {
		return positions.get_data();
	}

	std::vector<GLfloat>& get_normals() {
		return normals.get_data();
	}

	std::vector<GLint>& get_colors() {
		return colors.get_data();
	}

	std::vector<unsigned int>& get_indices() {
		return indices.get_data();
	}

	virtual int get_indices_count() { 
		return indices.get_data().size();
	};

	virtual void load() {
		positions.load();
		normals.load();
		colors.load();
		indices.load();
	};

	virtual void unload() {
		positions.unload();
		normals.unload();
		colors.unload();
		indices.unload();
	};

	virtual void bind() {
		positions.bind();
		normals.bind();
		colors.bind();
		indices.bind();
	};

	void unbind() {
		positions.unbind();
		normals.unbind();
		colors.unbind();
		indices.unbind();
	};
};