#pragma once

class Runner;
class Entity
{
public:
	Entity();
	~Entity();
	int id;
	bool started;
	bool removed;
	virtual void update();
	virtual void start();
	virtual void remove();
	Runner *runner;
};

