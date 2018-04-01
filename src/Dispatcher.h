#pragma once
#include "Worker.h"

class Dispatcher {
private:
	std::map<int, Worker *> map;
	std::vector<int> keys;
	int counter = 0;

public:
	void add(Worker *worker) {
		worker->id = ++counter;
		map[worker->id] = worker;
		keys.push_back(worker->id);
	};
	void update() {
		for(auto key : keys) {
			Worker *worker = map[key];
			worker->run();
		}

		for(auto key : keys) {
			delete map[key];
			map.erase(key);
		}

		keys.clear();
	};
};