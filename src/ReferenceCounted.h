#pragma once

#include <exception>

class ReferenceCounted {
public:
	int count = 1;

	void grab() {
		count++;
	}

	void drop() {
		if (count == 0) {
			throw std::exception("invalid drop");
		}

		count--;

		if (count == 0) {
			delete this;
		}
	}
};