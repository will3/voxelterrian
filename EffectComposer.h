#pragma once
#include "Renderer.h"
#include <vector>
#include "RenderTarget.h"

class EffectComposer {
private:
	std::vector<Pass *> passes;
	RenderTarget *writeBuffer;
	RenderTarget *readBuffer;

public:
	Renderer *renderer;
	EffectComposer(Renderer *renderer) : renderer(renderer) {
		writeBuffer = new RenderTarget(renderer->window_width, renderer->window_height);
		readBuffer = new RenderTarget(renderer->window_width, renderer->window_height);
	}

	void add_pass(Pass *pass) {
		passes.push_back(pass);
	}

	void render() {
		for (auto pass : passes) {
			pass->render(renderer, writeBuffer, readBuffer);

			if (pass->needsSwap) {
				swapBuffers();
			}
		}
	}

	void swapBuffers() {
		auto temp = readBuffer;
		readBuffer = writeBuffer;
		writeBuffer = temp;
	}
};

class Pass {
public:
	bool renderToScreen = false;
	bool needsSwap = true;
	virtual void render(Renderer *renderer, RenderTarget *writeBuffer, RenderTarget *readBuffer) {}
};