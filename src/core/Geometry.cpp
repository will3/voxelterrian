#include "Geometry.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Geometry::Geometry()
{
}

Geometry::~Geometry()
{
	unload();
}