#pragma once
#include "EffectComposer.h"
#include "common\shader.hpp"
#include "RenderTarget.h"

class CopyPass : public Pass {
	GLuint quad_programID;
	GLuint texID;
	GLuint quad_vertexbuffer;

	void load() {
		// The fullscreen quad's FBO
		static const GLfloat g_quad_vertex_buffer_data[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
		};

		glGenBuffers(1, &quad_vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

		// Create and compile our GLSL program from the shaders
		quad_programID = LoadShaders("shaders/copy.vertexshader", "shaders/copy.fragmentshader");
		texID = glGetUniformLocation(quad_programID, "renderedTexture");
	}

	void render(Renderer *renderer, RenderTarget *writeBuffer, RenderTarget *readBuffer) override {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (renderToScreen) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, writeBuffer->FramebufferName);
		}

		// Use our shader
		glUseProgram(quad_programID);
	
		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, readBuffer->renderedTexture);
		// Set our "renderedTexture" sampler to use Texture Unit 0
		glUniform1i(texID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

		glDisableVertexAttribArray(0);
	}
};