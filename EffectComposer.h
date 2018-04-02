#pragma once
#include "Renderer.h"
#include <vector>

class EffectComposer {
private:
	std::vector<Pass *> passes;
public:
	Renderer *renderer;
	EffectComposer(Renderer *renderer) : renderer(renderer) {}
	void add_pass(Pass *pass) {
		passes.push_back(pass);
	}

	void render() {
		for (auto pass : passes) {
			pass->render();
		}
	}
};

class Pass {
public:
	void render() {

	}
};