#pragma once
#include "Pass.h"
#include "Scene.h"
#include "ShaderMaterial.h"
#include "DepthShader.h"

class ShadowMap {
public:
	Camera * camera;
	ShaderMaterial *depthMaterial;
	RenderTarget *renderTarget;

	ShadowMap(float width, float height, float near, float far, int textureWidth, int textureHeight) {
		camera = new OrthographicCamera(-width / 2, width / 2, -height / 2, height / 2, near, far);
		renderTarget = new RenderTarget(textureWidth, textureHeight);
		depthMaterial = new ShaderMaterial(new DepthShader());
	}

	GLuint getShadowMap() {
		return renderTarget->depthTexture;
	}

	~ShadowMap() {
		delete renderTarget;
		delete depthMaterial;
	}

	void render(Renderer *renderer, Scene *scene) {
		scene->override_material = depthMaterial;
		renderer->render(scene, camera, renderTarget);
		scene->override_material = 0;
	}
};