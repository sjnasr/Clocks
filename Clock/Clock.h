#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include "Configuration.h"
class ClockClass {
	//All the variables, clean this up today!!
private:
	SYSTEMTIME sysTime;
	RECT myRectangleTime, test;
	std::string time, date;
	Configuration checking;
	bool millisecondBool;
public:
	void DisplayTime(HDC hdc);
	void DisplayDate(HDC hdc);
	void checkingMilliBool();
	void formatString();
	std::string getTime();
	std::string getDate();
	RECT getRect();
	//std::vector<int> stringSeparation(std::string input);
	//void differenceBetweenStartAndStop(HDC hdc, std::string startTime, std::string stopTime); //figure out the return type
};