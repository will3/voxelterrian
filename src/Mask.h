#pragma once
#include <vector>
#include <array>
#define max_light_value 15

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

	Mask(int size, int i, int d, int front):size(size), i(i), d(d), front(front) {
		this->size = size;
		data.resize(size * size);
	}

	bool is_empty(int j, int k) {
		int index = j * size + k;
		return data[index].v == 0;
	}

	MaskValue get(int j, int k) {
		int index = j * size + k;
		return data[index];
	}

	void set(int j, int k, MaskValue& v) {
		int index = j * size + k;
		data[index] = v;
	}

	bool stop_merge(MaskValue& c, MaskValue& next) {
		return next.v != c.v || next.has_ao() || next.lighting != c.lighting;
	}

	/*void copy(SMeshBuffer *buffer, int x, int y, int w, int h, int ao0, int ao1, int ao2, int ao3, int lighting) {
		float ao_strength = 0.1f;
		auto &vertices = buffer->Vertices;
		int index = vertices.size();
		auto &indices = buffer->Indices;

		Coord3 v0 = Coord3(i, x, y).rotate(d);
		Coord3 v1 = Coord3(i, x + w, y).rotate(d);
		Coord3 v2 = Coord3(i, x + w, y + h).rotate(d);
		Coord3 v3 = Coord3(i, x, y + h).rotate(d);

		vertices.push_back(S3DVertex(vector3df(v0.i, v0.j, v0.k), vector3df(), SColor(255, 255, 255, 255), vector2df(1.0 - (ao0 / 3.0 * ao_strength), lighting / 16.0)));
		vertices.push_back(S3DVertex(vector3df(v1.i, v1.j, v1.k), vector3df(), SColor(255, 255, 255, 255), vector2df(1.0 - (ao1 / 3.0 * ao_strength), lighting / 16.0)));
		vertices.push_back(S3DVertex(vector3df(v2.i, v2.j, v2.k), vector3df(), SColor(255, 255, 255, 255), vector2df(1.0 - (ao2 / 3.0 * ao_strength), lighting / 16.0)));
		vertices.push_back(S3DVertex(vector3df(v3.i, v3.j, v3.k), vector3df(), SColor(255, 255, 255, 255), vector2df(1.0 - (ao3 / 3.0 * ao_strength), lighting / 16.0)));

		if (front) {
			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index + 2);
			indices.push_back(index + 2);
			indices.push_back(index + 3);
			indices.push_back(index);
		}
		else {
			indices.push_back(index + 2);
			indices.push_back(index + 1);
			indices.push_back(index);
			indices.push_back(index);
			indices.push_back(index + 3);
			indices.push_back(index + 2);
		}
	}*/

	//void get_quads(SMeshBuffer *buffer) {
	//	int n = 0;
	//	MaskValue c;
	//	int w, h;

	//	for (int j = 0; j < size; j++) {
	//		for (int i = 0; i < size; ) {
	//			c = data[n];
	//			
	//			if (c.v == 0) {
	//				i++;
	//				n++;
	//				continue;
	//			}

	//			// Check AO
	//			if (c.has_ao()) {
	//				copy(buffer, j, i, 1, 1, c.ao0, c.ao1, c.ao2, c.ao3, c.lighting);
	//				i++;
	//				n++;
	//				continue;
	//			}

	//			int lighting = c.lighting;

	//			// Compute width
	//			for (w = 1; i + w < size; ++w) {
	//				MaskValue &next = data[n + w];
	//				if (stop_merge(c, next)) {
	//					break;
	//				}
	//			}

	//			// Compute height
	//			bool done = false;
	//			for (h = 1; j + h < size; h++) {
	//				for (int k = 0; k < w; k++) {
	//					MaskValue &next = data[n + k + h * size];
	//					if (stop_merge(c, next)) {
	//						done = true;
	//						break;
	//					}
	//				}
	//				if (done) {
	//					break;
	//				}
	//			}

	//			// Add Quad
	//			copy(buffer, j, i, h, w, 0, 0, 0, 0, lighting);

	//			//Zero-out mask
	//			for (int l = 0; l < h; l++) {
	//				for (int k = 0; k < w; k++) {
	//					data[n + k + l * size] = 0;
	//				}
	//			}

	//			i += w; n += w;
	//		}
	//	}
	//}
};