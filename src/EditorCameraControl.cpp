#include "EditorCameraControl.h"
#include "Runner.h"
#include "Node.h"

EditorCameraControl::EditorCameraControl() {
}

EditorCameraControl::~EditorCameraControl() {
}

void EditorCameraControl::update() {
	camera->position.z += speed * runner->time_step;
	camera->target = 
		{ camera->position.x, 0, camera->position.z + 200 };

	terrian->player_position = camera->target;
}