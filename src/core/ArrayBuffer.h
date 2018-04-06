#pragma once
#include <vector>
#include <gl\glew.h>

template<typename T, GLenum type_enum>
class ArrayBuffer {
private:
	std::vector<T> data;
public:
	GLuint handle = 0;
	bool is_int;
	int location = 0;
	int elements_per_vertex = 3;

	std::vector<T>& get_data() {
		return data;
	}

	ArrayBuffer(int location, int elements_per_vertex) : location(location), elements_per_vertex(elements_per_vertex) {
		// Not complete
		is_int = type_enum == GL_INT;
	}

	void load() {
		glGenBuffers(1, &handle);
		glBindBuffer(GL_ARRAY_BUFFER, handle);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
	};

	void unload() {
		glDeleteBuffers(1, &handle);
	}

	void bind() {
		if (is_int) {
			glEnableVertexAttribArray(location);
			glBindBuffer(GL_ARRAY_BUFFER, handle);
			glVertexAttribIPointer(location, elements_per_vertex, type_enum, 0, (void*)0);
		}
		else {
			glEnableVertexAttribArray(location);
			glBindBuffer(GL_ARRAY_BUFFER, handle);
			glVertexAttribPointer(location, elements_per_vertex, type_enum, false, 0, (void*)0);
		}
	}

	void unbind() {
		glDisableVertexAttribArray(location);
	}
};