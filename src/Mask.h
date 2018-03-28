#pragma once
#include <vector>
#include <array>
#define max_light_value 15

class VoxelMesh;

typedef __int8 ao_type;

class MaskValue {
public:
	int v = 0;
	__int8 ao0;
	__int8 ao1;
	__int8 ao2;
	__int8 ao3;
	__int8 lighting = max_light_value;

	bool has_ao() {
		return  ao0 || ao1 || ao2 || ao3;
	}
	MaskValue(int v, int ao0, int ao1, int ao2, int ao3, int light) : v(v), ao0(ao0), ao1(ao1), ao2(ao2), ao3(ao3), lighting(light) { };
	MaskValue(int v) : v(v) { };
	MaskValue() {};
};

class Mask {
public:
	std::vector<MaskValue> data;
	int size;
	bool front = false;
	int i = 0;
	int d = 0;

	Mask(int size, int i, int d, int front);

	bool is_empty(int j, int k);

	MaskValue get(int j, int k);

	void set(int j, int k, MaskValue& v);

	bool stop_merge(MaskValue& c, MaskValue& next);

	void copy_quad(VoxelMesh * buffer, int x, int y, int w, int h, int ao0, int ao1, int ao2, int ao3, int lighting);

	void get_quads(VoxelMesh *mesh);
};