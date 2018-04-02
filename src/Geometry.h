#pragma once
#include <GL\glew.h>
#include <vector>

class Geometry
{
public:
	bool loaded = false;
	std::vector<unsigned int> indices;

	virtual void load() {};
	virtual void unload() {};
	virtual void bind() {};
	void unbind() {};

	Geometry();
	~Geometry();
};