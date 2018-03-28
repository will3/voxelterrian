#pragma once
#include "linalg.h"
#include <sstream>

typedef linalg::vec<float, 3> float3;

struct Coord2 {
	int i = 0;
	int j = 0;

	Coord2() {};
	Coord2(int i, int j) :i(i), j(j) {};

	bool operator == (const Coord2 &o) const {
		return i == o.i && j == o.j;
	}

	bool operator < (const Coord2 &o) const {
		if (i == o.i) {
			return j < o.j;
		}
		return i < o.i;
	}

	Coord2 operator - (const Coord2 &o) const {
		return { i - o.i, j - o.j };
	}

	Coord2 operator + (const Coord2 &o) const {
		return { i + o.i, j + o.j };
	}

	Coord2 operator * (int num) const {
		return { i * num, j * num };
	}
};

struct Coord3 {
	int i;
	int j;
	int k;

	Coord3() {};
	Coord3(int i, int j, int k) : i(i), j(j), k(k) {};

	bool operator == (const Coord3 &o) const {
		return i == o.i && j == o.j && k == o.k;
	}

	bool operator != (const Coord3 &o) const {
		return i != o.i || j != o.j || k != o.k;
	}

	bool operator < (const Coord3 &o) const {
		if (i == o.i) {
			if (j == o.j) {
				return k < o.k;
			}
			return j < o.j;
		}
		return i < o.i;
	}

	Coord3 operator - (const Coord3 &o) const {
		return { i - o.i, j - o.j, k - o.k };
	}

	Coord3 operator + (const Coord3 &o) const {
		return { i + o.i, j + o.j, k + o.k };
	}

	Coord3 operator * (int num) const {
		return { i * num, j * num, k * num };
	}

	Coord3 operator / (int num) const {
		return { i / num, j / num, k / num };
	}

	Coord2 ik() {
		return { i, k };
	}

	int& operator[] (const int index) {
		if (index == 0) {
			return i;
		}
		else if (index == 1) {
			return j;
		}
		return k;
	}

	Coord3 rotate(int d) {
		if (d == 0) {
			return { i,j,k };
		}
		else if (d == 1) {
			return { k, i, j };
		}
		return { j, k, i };
	}
};

typedef Coord3 chunk_key;

namespace std {
	template <>
	struct hash<Coord3>
	{
		std::size_t operator()(const Coord3& k) const
		{
			using std::hash;

			std::size_t v = 23;
			
			v += hash<int>()(k.i) * 17;
			v += hash<int>()(k.j) * 17;
			v += hash<int>()(k.k) * 17;

			return v;
		}
	};
}