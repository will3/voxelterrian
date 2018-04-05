#include "gtest/gtest.h"
#include "Field3.h"

TEST(Field3, Test) {
	Field3<float> f(3);

	f.set(0, 0, 0, 1.0);
	f.set(1, 0, 0, 2.0);
	f.set(2, 0, 0, 3.0);
	f.set(3, 0, 0, 4.0);

}