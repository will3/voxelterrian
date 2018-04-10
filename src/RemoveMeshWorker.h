#include "Worker.h"
#include "Mesh.h"

// It's not safe to delete gl resources in other threads
class RemoveMeshWorker : public Worker {
public:
	Mesh * mesh;
	RemoveMeshWorker(Mesh *mesh) :mesh(mesh) {}
	void run() {
		mesh->geometry->unload();
		delete mesh->geometry;

		mesh->remove_self();
		delete mesh;
	}
};