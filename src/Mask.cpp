#include "Mask.h"
#include "Mesh.h"
#include "types.h"

Mask::Mask(int size, int i, int d, int front) :size(size), i(i), d(d), front(front) {
	this->size = size;
	data.resize(size * size);
}

Mask::~Mask()
{
}

bool Mask::is_empty(int j, int k) {
	int index = j * size + k;
	return data[index].v == 0;
}

MaskValue Mask::get(int j, int k) {
	int index = j * size + k;
	return data[index];
}

void Mask::set(int j, int k, MaskValue & v) {
	int index = j * size + k;
	data[index] = v;
}