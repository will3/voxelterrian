#pragma once
#include "Pass.h"
#include "Scene.h"
#include "ShaderMaterial.h"
#include "DepthShader.h"

class ShadowPass : public Pass {
public:
	Scene *scene;
	Camera *camera;
	ShaderMaterial *depthMaterial = new ShaderMaterial(new DepthShader());

	ShadowPass(Scene *scene, float size, float near, float far) :scene(scene) {
		camera = new OrthographicCamera(-size / 2, size / 2, -size / 2, size / 2, near, far);
		depthMaterial = new ShaderMaterial(new DepthShader());
	};

	void render(Renderer *renderer, RenderTarget *writeBuffer, RenderTarget *readBuffer) override {

		scene->override_material = depthMaterial;

		if (renderToScreen) {
			renderer->render(scene, camera);
		}
		else {
			renderer->render(scene, camera, writeBuffer);
		}

		scene->override_material = 0;

		scene->currentShadowPass = this;
	}
};