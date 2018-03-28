#pragma once
#include <deque>
#include <string>

class Logger
{
private:
	static Logger* instance;
	std::deque<std::string *> messages;

public:
	static Logger* get_instance();

	void log(std::string message);
	bool has_message();
	std::string get_message();
	std::string* get_message_ref();
	void remove_message();

	Logger();
	~Logger();
};

