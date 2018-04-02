#pragma once
#include "GL/glew.h"
#include "RenderTarget.h"
#include "ShaderMaterial.h"
#include "DepthShader.h"
#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>

class ShadowMap {
private:
	glm::vec3 lightInvDir;

public:
	int width;
	int height;

	RenderTarget *renderTarget;
	ShaderMaterial *depthMaterial;
	Camera *camera;
	Renderer *renderer = new Renderer();

	ShadowMap(float width, float height, float near, float far) : width(width), height(height) {
		depthMaterial = new ShaderMaterial(new DepthShader());
		renderTarget = new RenderTarget(width, height);
		camera = new OrthographicCamera(-width / 2, width / 2, -height / 2, height / 2, near, far);
	}

	void render(Scene *scene) {
		scene->override_material = depthMaterial;
		renderer->render(scene, camera, renderTarget);
		scene->override_material = 0;
	}

	void renderToScreen(Scene *scene) {
		scene->override_material = depthMaterial;

		renderer->render(scene, camera);
		scene->override_material = 0;
	}

	~ShadowMap() {
		delete depthMaterial;
		delete renderer;
		delete camera;
		delete renderTarget;
	}
};