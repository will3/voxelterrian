#pragma once

#include "Entity.h"
#include "Terrian.h"

class EditorCameraControl : public Entity
{
public:
	EditorCameraControl();
	~EditorCameraControl();

	Camera *camera;
	Terrian *terrian;

	float speed = 20.0;

	void update();
};

