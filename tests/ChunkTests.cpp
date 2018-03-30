#include "gtest/gtest.h"
#include <Chunk.h>

TEST(Chunk, Set) {
	Chunk* chunk = new Chunk();
	chunk->set({ 1, 2, 3 }, 4);
	EXPECT_EQ(chunk->get({ 1, 2, 3 }), 4);
}

TEST(Chunk, SetMillion) {
	Chunk* chunk = new Chunk(100);
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j< 100; j++) {
			for (int k = 0; k < 100; k++) {
				chunk->set({ i, j, k }, 1);
			}
		}
	}
}