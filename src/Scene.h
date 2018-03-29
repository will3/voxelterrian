#include <vector>
#include "Node.h"
#include <unordered_set>

namespace kst {
	class Scene {
	public:
		std::unordered_set<Node *> nodes;
		void add(Node *node);
		void remove(Node *node);
	};
}