#include "Logger.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "Definitions.h"
#include <mutex>
#include <time.h>

using namespace std;
using namespace std::chrono;

Logger* Logger::_this = nullptr;
ofstream Logger::logFile = ofstream("./log.txt", ofstream::app);
mutex Logger::mut = mutex();

Logger::Logger()
{
	cout << "DiosMMO Server. Versao " << SERVER_VERSION	<< endl;;
}

Logger::Logger(const Logger& logger)
{
}

Logger::Logger(const Logger&& logger)
{
}

Logger* Logger::getInstance()
{
	if(_this == nullptr)
		_this = new Logger();

    return _this;
}

void Logger::print(const string& msg)
{
	if(_this == nullptr)
		_this = new Logger();

	cout << msg << endl;
}

void Logger::log(const string& func, const string& msg)
{
	time_t cnow = chrono::system_clock::to_time_t(chrono::system_clock::now());
	string time = ctime(&cnow);
	time = time.substr(0, time.size()-5); //Remove o ano
	time.erase(time.end()-1);

	auto clock = chrono::system_clock::now();
	auto fraction = clock - chrono::time_point_cast<chrono::seconds>(clock);
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(fraction);

	size_t funcNameStart = func.find(" ");
	string funcName = func.substr(funcNameStart+1); //Apaga o tipo de retorno da função
	funcName = funcName.substr(0, funcName.size()-2); //Apaga o () no final

	mut.lock();
	logFile	<< time << ":" << milliseconds.count() % 1000 << " >> " << funcName << ": " << msg + "\n";
	logFile.flush();
	mut.unlock();
}

void Logger::destroy()
{
	if(_this != nullptr)
		delete _this;
}