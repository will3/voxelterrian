#include "gtest/gtest.h"
#include <Chunks.h>

TEST(Chunks, Set) {
	Chunks* chunks = new Chunks();
	chunks->set(Coord3(1, 2, 3), 4);

	EXPECT_EQ(chunks->get(Coord3(1, 2, 3)), 4);
}

TEST(Chunks, Set2) {
	Chunks* chunks = new Chunks();
	chunks->set(Coord3(111, 222, 333), 12);

	EXPECT_EQ(chunks->get(Coord3(111, 222, 333)), 12);
}

TEST(Chunks, CreateChunk) {
	Chunks* chunks = new Chunks();
	Chunk *a = chunks->get_or_create_chunk(Coord3(0, 0, 0));
	Chunk *b = chunks->get_or_create_chunk(Coord3(0, 0, 0));

	EXPECT_TRUE(a == b);
}

TEST(Chunks, CreateChunkMultiple) {
	Chunks* chunks = new Chunks();
	chunks->get_or_create_chunk(Coord3(0, 0, 0));
	chunks->get_or_create_chunk(Coord3(32, 0, 0));
	chunks->get_or_create_chunk(Coord3(0, 32, 0));
	chunks->get_or_create_chunk(Coord3(0, 0, 32));

	EXPECT_EQ(chunks->get_coords().size(), 4);
}

TEST(Chunks, DontCreateDuplicates) {
	Chunks *chunks = new Chunks();
	chunks->get_or_create_chunk({ 0,0,0 });
	chunks->get_or_create_chunk({ 0,0,0 });

	EXPECT_EQ(chunks->get_coords().size(), 1);
}

TEST(Chunks, MapPerformance) {
	Chunks *chunks = new Chunks();
	int size = 20;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				Coord3 coord = { i, j, k };
				chunks->get_or_create_chunk(coord);
			}
		}
	}
}