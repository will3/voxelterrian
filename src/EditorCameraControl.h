#pragma once

#include "Entity.h"
#include "Camera.h"

class EditorCameraControl : public Entity
{
public:
	EditorCameraControl();
	~EditorCameraControl();

	Camera *camera;

	float speed = 0.0;

	void update();
};

