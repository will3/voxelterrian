#pragma once
#include "EffectComposer.h"
#include "RenderTarget.h"
#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"

class RenderPass : Pass {
	Scene *scene;
	Camera *camera;

	RenderPass(Scene *scene, Camera *camera) : scene(scene), camera(camera) {}

	void render(Renderer *renderer, RenderTarget *writeBuffer, RenderTarget *readBuffer) override {
		renderer->render(scene, camera, renderToScreen ? 0 : readBuffer);
	}
};