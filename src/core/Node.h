#pragma once

#include "types.h"
#include "Camera.h"
#include <unordered_set>
#include <glm/gtc/quaternion.hpp>

class Scene;

class Node {
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::mat4 matrix;

	Scene *scene = 0;
	Node *parent = 0;
	std::unordered_set<Node *> nodes;

	virtual void render(Camera *camera) {}

	virtual void add(Node *node);
	void remove(Node *node);

	void update_matrix();

	void remove_self();
};
