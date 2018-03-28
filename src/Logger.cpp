#include "Logger.h"

Logger* Logger::instance = 0;

Logger* Logger::get_instance()
{
	if (instance == 0)
	{
		instance = new Logger();
	}

	return instance;
}

Logger::Logger()
{
}


Logger::~Logger()
{
}

void Logger::log(std::string message) {
	messages.push_back(new std::string(message));
}

std::string Logger::get_message() {
	return *messages.front();
}

std::string* Logger::get_message_ref() {
	return messages.front();
}

void Logger::remove_message() {
	messages.pop_front();
}

bool Logger::has_message() {
	return messages.size() > 0;
}