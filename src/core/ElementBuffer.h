#pragma once
#include <vector>
#include <GL/glew.h>

class ElementBuffer {
private:
	std::vector<unsigned int> data;
public:
	GLuint handle = 0;

	std::vector<unsigned int>& get_data() {
		return data;
	}

	void load() {
		glGenBuffers(1, &handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), &data[0], GL_STATIC_DRAW);
	}

	void unload() {
		glDeleteBuffers(1, &handle);
	}

	void bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	}

	void unbind() {
	}
};