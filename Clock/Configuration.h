#pragma once
#include <Windows.h>
#include <string>

struct config
{
	int loggingRate;
	char logFile[256];
	bool milliEnable;
	bool logEnable;
};
class Configuration {
private:
	char millisecondEnable[4096];
	char loggingEnable[256];
	unsigned int loggingRate;
	char logFile[256];
	LPCSTR path = ".\\Configuration.ini";
	bool changing, milliEnable, logEnable;

public:
	Configuration(); //Constructor
	~Configuration(); //Destructor

	//Need a function to set and read the config
	void setConfig(std::string key);
	void setLoggingOptions(char input[]);
	void readConfig();
	void loadConfig();

	//need a function to change string to bool
	bool toBoolean(std::string str);

	//Create get methods so I can retrieve the value
	bool getMillisecondBool() { return milliEnable; }
	std::string logFileAppendOrOverwrite() { std::string aoo(logFile); return aoo; } //aoo = append or overwrite
	bool getLoggingValue() { return logEnable; }
	int getLoggingRate() { return loggingRate; }
};