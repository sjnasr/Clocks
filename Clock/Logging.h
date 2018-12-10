#pragma once
#include "Configuration.h"
#include "Clock.h"
#include <fstream>

class LoggingEnabled
{
private:
	Configuration config;
	std::ofstream lapLogging;
public:
	LoggingEnabled();
	~LoggingEnabled();

	//create Write function
	void writeToLog(std::string saveTime, std::string saveDate);
};