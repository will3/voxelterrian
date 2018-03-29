#pragma once

#include "types.h"
#include "ReferenceCounted.h"
#include "Camera.h"
#include <unordered_set>

class Node : public ReferenceCounted {
public:
	float3 position;
	Node *parent = 0;
	std::unordered_set<Node *> nodes;

	virtual void render(Camera *camera) {}

	void add(Node *node);
	void remove(Node *node);
};
