#pragma once

#include <vector>
#include "Node.h"
#include <unordered_set>
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include <set>
class Scene : public Node {
public:
	std::set<DirectionalLight *> directional_lights;
	std::set<AmbientLight *> ambient_lights;
	bool override_material = false;

	void add(Node *node) override {
		Node::add(node);

		node->scene = this;

		DirectionalLight *directional_light = dynamic_cast<DirectionalLight *>(node);
		if (directional_light != NULL) {
			directional_lights.insert(directional_light);
		}
	
		AmbientLight *ambientLight = dynamic_cast<AmbientLight *>(node);
		if (ambientLight != NULL) {
			ambient_lights.insert(ambientLight);
		}
	}

	virtual void render(Camera *camera) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (camera != 0) {
			camera->update_view_matrix();
		}

		for (Node *node : nodes) {
			node->update_matrix();
			node->render(camera);
		}
	}
};