#include "Logging.h"
#include <exception>
#include <string>
#include <iostream>

using namespace std;

LoggingEnabled::LoggingEnabled()
{
	string value = config.logFileAppendOrOverwrite();
	config.readConfig();
	//Overwrite or append
	try {
		if (config.logFileAppendOrOverwrite() == "Overwrite")
		{
			lapLogging.open("log.txt", ios::out | ios::trunc);
		}
		else if (config.logFileAppendOrOverwrite() == "Append")
		{
			lapLogging.open("log.txt", ios::out | ios::app);
		}
	}
	catch (exception &e)
	{
		cerr << "Error while trying to log a message:\n" << e.what() << endl;
	}
}

LoggingEnabled::~LoggingEnabled()
{
	try {
		lapLogging.close();
	}
	catch (exception &e)
	{
		cerr << "Error while trying to log a message:\n" << e.what() << endl;
	}
}

void LoggingEnabled::writeToLog(string saveTime, string saveDate)
{
	config.readConfig();
	//need a timer so it'll or maybe system pause or something
	try {
		if (lapLogging.is_open())
		{
			lapLogging << saveTime << endl;
			lapLogging << saveDate << endl;
		}
	}
	catch (exception& e)
	{
		cerr << "Error while trying to log a message:\n" << e.what() << endl;
	}

}