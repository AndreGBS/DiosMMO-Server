#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

using namespace std;

class Logger
{
public:
	static void print(const string& msg);
	static void log(const string& func, const string& msg);
	static Logger* getInstance();
	static void destroy();

private:
	static Logger* _this;
	static ofstream logFile;
	static mutex mut;
	Logger();
	Logger(const Logger& logger);
	Logger(const Logger&& logger);
};

#endif