#include "Configuration.h"
#include <sstream>
#include <iostream>
using namespace std;

//THIS CAN BE OPTIMIZED AND DONE IN A MORE EFFICIENT, BETTER WAY
//Store path in a string
//Use a loop to get next section/value, maybe 2? line and each string
//loop(line):
	//loop(string)
		//GetPrivateProfileString(...)

//Don't get too caught up with optimizing
//Get basic logic down and make sure I can edit the values
Configuration::Configuration() 
{
	/*OPTIMIZE THIS AREA USING FOR LOOP*/
	readConfig();

	//Add a check for failure
}

Configuration::~Configuration()
{
	//Close ini file
}

void Configuration::readConfig()
{
	long retVal = GetPrivateProfileString("Millisecond", "EnableMillisecond", "", millisecondEnable, 255, path);
	retVal = GetPrivateProfileString("Logging", "EnableLogging", "", loggingEnable, 255, path); //WORKED YAY.
	loggingRate = GetPrivateProfileInt("LoggingRate", "LoggingRate", 0, path);
	retVal = GetPrivateProfileString("LogFile", "LogFile", "", logFile, 255, path);

	milliEnable = toBoolean(millisecondEnable);
	logEnable = toBoolean(loggingEnable);
}

//Optimize
//for boolean
void Configuration::setConfig(string key)
{
	if (key == "millisecondEnable") {

		//start with milliseconds
		if (milliEnable == false) {
			//millisecondEnable = TRUE;
			WritePrivateProfileString("Millisecond", "EnableMillisecond", "true", path);
		}
		else if(milliEnable == true) {
			WritePrivateProfileString("Millisecond", "EnableMillisecond", "false", path);
		}
	}
	else if (key == "loggingEnable") {
		//Enable logging
		if (logEnable == false)
		{
			WritePrivateProfileString("Logging", "EnableLogging", "true", path);
		}
		else
		{
			WritePrivateProfileString("Logging", "EnableLogging", "false", path);
		}

	}
	
}

void Configuration::setLoggingOptions(char input[])
{
	if (strcmp( "3 Minutes", input) == 0 /*&& loggingRate != 180000*/) //compare string and logging rate, if both are true then change value
	{
		WritePrivateProfileString("LoggingRate", "LoggingRate", "180000", path);
	}
	else if (strcmp("5 Minutes", input) == 0 /*&& loggingRate != 300000*/)
	{
		WritePrivateProfileString("LoggingRate", "LoggingRate", "300000", path);
	}
	else if (strcmp("7 Minutes", input) == 0 /*&& loggingRate != 420000*/)
	{
		WritePrivateProfileString("LoggingRate", "LoggingRate", "420000", path);
	}
	else
	{
		cout << "An error has occurred" << endl;
	}

	if (strcmp("Append", input) == 0)
	{
		WritePrivateProfileString("LogFile", "LogFile", "Append", path);
	}
	else if (strcmp("Overwrite", input) == 0)
	{
		WritePrivateProfileString("LogFile", "LogFile", "Overwrite", path);
	}
}


//
//PURPOSE: Change string to boolean
//
bool Configuration::toBoolean(string str)
{
	istringstream iss(str);
	iss >> boolalpha >> changing;
	return changing;
}