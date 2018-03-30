#include "gtest/gtest.h"
#include <Terrian.h>

//TEST(Terrian, Generate) {
//	Terrian *terrian = new Terrian();
//	terrian->generate({ 0, 0, 0 });
//}

//TEST(Terrian, ShouldChunk) {
//	Terrian *terrian = new Terrian();
//	terrian->chunking_dis = 4;
//	terrian->player_position = float3(0, 0, 0);
//	Chunk *chunk = new Chunk(32, { 32 * 5, 0, 0 });
//	EXPECT_FALSE(terrian->should_chunk(chunk));
//}

//TEST(Terrian, ShouldChunk2) {
//	Terrian *terrian = new Terrian();
//	terrian->chunking_dis = 4;
//	terrian->player_position = float3(0, 0, 0);
//	Chunk *chunk = new Chunk(32, { 32 * 4, 0, 0 });
//	EXPECT_TRUE(terrian->should_chunk(chunk));
//}