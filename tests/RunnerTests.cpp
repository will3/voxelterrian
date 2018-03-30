#include "gtest/gtest.h"
#include <Runner.h>

TEST(Runner, TestAdd) {
	Runner *runner = new Runner();
	int id = runner->add(new Entity());
	EXPECT_TRUE(runner->has(id));
}

TEST(Runner, UpdateStartEntity) {
	Runner *runner = new Runner();
	Entity *entity = new Entity();
	runner->add(entity);
	runner->update();
	EXPECT_TRUE(entity->started);
}

TEST(Runner, UpdateRemoveEntity) {
	Runner *runner = new Runner();
	Entity *entity = new Entity();
	int id = runner->add(entity);
	runner->remove(id);
	EXPECT_TRUE(entity->removed);
}

TEST(Runner, UpdateRemoveEntityFromMap) {
	Runner *runner = new Runner();
	Entity *entity = new Entity();
	int id = runner->add(entity);
	runner->remove(id);
	EXPECT_TRUE(runner->has(id));
	runner->update();
	EXPECT_FALSE(runner->has(id));
}
