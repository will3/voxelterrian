#include "Scene.h"

void kst::Scene::add(Node * node)
{
	nodes.insert(node);
	node->grab();
}

void kst::Scene::remove(Node * node)
{
	node->drop();
	nodes.erase(node);
}
