#pragma once
#include <GL/glew.h>
#include <stdio.h>
#include "common\shader.hpp"
#include "DirectionalLight.h"
#include "ShadowMaterial.h"

class ShadowMap {
public:
	GLuint handle = 0;
	GLuint depthTexture;
	ShadowMaterial *shadow_material = new ShadowMaterial();

	void draw(DirectionalLight *light) {
		if (!shadow_material->loaded) {
			shadow_material->load();
			shadow_material->loaded = true;
		}

		glm::vec3 lightInvDir = light->direction;
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
		glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 depthModelMatrix = glm::mat4(1.0);
		shadow_material->depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

		shadow_material->bind_uniforms();
	}

	void draw() {
		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		GLuint FramebufferName = 0;
		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

		// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
		GLuint depthTexture;
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

		// No color output in the bound framebuffer, only depth.
		glDrawBuffer(GL_NONE);

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("frame buffer not ok");
		}
	}

	~ShadowMap() {
		delete shadow_material;
	}
};