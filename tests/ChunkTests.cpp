#include "gtest/gtest.h"
#include <Chunk.h>

TEST(Chunk, Set) {
	Chunk* chunk = new Chunk();
	chunk->set({ 1, 2, 3 }, 4);
	EXPECT_EQ(chunk->get({ 1, 2, 3 }), 4);
}

TEST(Chunk, SetMillion) {
	Chunk* chunk = new Chunk();
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j< CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				chunk->set({ i, j, k }, 1);
			}
		}
	}
}