#pragma once

#include "gtest/gtest.h"
#include "Camera.h"

TEST(OrthoGraphicCamera, projection_matrix) {
	float left = -1, right = 1, top = 1, bottom = -1, near = 1, far = 3;
	auto camera = new OrthographicCamera(left, right, bottom, top, near, far);
	auto pMatrix = camera->Projection;

	// orthographic projection is given my the 4x4 Matrix
	// 2/r-l		0			 0		-(l+r/r-l)
	//   0		2/t-b		 0		-(t+b/t-b)
	//   0			0		-2/f-n	-(f+n/f-n)
	//   0			0			 0				1

	ASSERT_EQ(pMatrix[0][0], 2 / (right - left));
	ASSERT_EQ(pMatrix[1][1], 2 / (top - bottom));
	ASSERT_EQ(pMatrix[2][2], -2 / (far - near));
	ASSERT_EQ(pMatrix[3][0], -((right + left) / (right - left)));
	ASSERT_EQ(pMatrix[3][1], -((top + bottom) / (top - bottom)));
	ASSERT_EQ(pMatrix[3][2], -((far + near) / (far - near)));
}