#include "Scene.h"
#include "Camera.h"

namespace kst {
	class Renderer {
		void render(Scene *scene, Camera *camera) {
			for (Node *node : scene->nodes) {
				render_node(node);
			}
		}

		void render_node(Node *node) {

		}
	};
}