#include "Node.h"

void Node::add(Node * node)
{
	if (node->parent != 0) {
		node->parent->remove(node);
	}

	nodes.insert(node);
	node->grab();
	node->parent = this;
}

void Node::remove(Node * node)
{
	node->drop();
	nodes.erase(node);
	node->parent = 0;
}