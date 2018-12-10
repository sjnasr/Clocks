#define _CRT_SECURE_NO_WARNINGS
#include "Clock.h"
#include <atlstr.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

void ClockClass::DisplayDate(HDC hdc)
{
	RECT myRectangle;
	int top, bottom, right, left; //variables used to create the rectangle for the date

	//Get local time
	GetLocalTime(&sysTime);

	char buffer[255];
	sprintf(buffer, "%d/%d/%d", sysTime.wMonth, sysTime.wDay, sysTime.wYear);
	date = buffer;

	SetTextColor(hdc, RGB(255, 105, 180));
	top = 50;
	bottom = 80;
	right = 80;
	left = 0;

	SetRect(&myRectangle, left, top, right, bottom);
	DrawText(hdc, date.c_str(), -1, &myRectangle, DT_CENTER); /*FIX ME, IT'S SO TINY*/
}

/*FIXME: Reorganize and comment. It's super messy
FIXME: Millisecond lingers when millisecond=false*/
void ClockClass::DisplayTime(HDC hdc) 
{
	int topTime, bottomTime, rightTime, leftTime; //variables used to create the rectangle for the time

	//Get local time
	GetLocalTime(&sysTime);

	SetTextColor(hdc, RGB(255, 105, 180));
	
	topTime = 70;
	bottomTime = 140;
	leftTime = 10;
	rightTime = 100;
	SetRect(&myRectangleTime, leftTime, topTime, rightTime, bottomTime);

	checkingMilliBool();
	formatString();
	
	DrawText(hdc, time.c_str(), -1, &myRectangleTime, DT_LEFT | DT_NOCLIP);


}

//Checks to see number is under 10 to fix flickering glitch

void ClockClass::formatString()
{
	char buffer[255];

	if (millisecondBool == true)
	{
		sprintf(buffer, "%02u:%02u:%02u:%02d", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
	}
	else
	{
		sprintf(buffer, "%02u:%02u:%002u", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	}
	time = buffer;
}

void ClockClass::checkingMilliBool()
{
	checking.readConfig();

	if (checking.getMillisecondBool() == true)
	{
		millisecondBool = true;
	}
	else
	{
		millisecondBool = false;
	}
}

string ClockClass::getTime()
{
	return time;
}

RECT ClockClass::getRect()
{
	return myRectangleTime;
}

string ClockClass::getDate()
{
	return date;
}


//Used to get the difference
//Issue with this approach is that there is delay
//Perhaps remove  and separate the string then find the difference instead
//vector<int> ClockClass::stringSeparation(string input)
//{
//	//Seperate point = :
//	//store in an array
//	vector<int> timeSeparated;
//	//char delimiters[] = ":";
//
//
//	//for (unsigned int i = 0; i <input.size(); i++)
//	//{
//
//	//	//input.erase(remove(input.begin(), input.end(), delimiters[i]), input.end());
//	//}
//	replace(input.begin(), input.end(), ':', ' ');
//	stringstream ss(input);
//	int temp;
//	while (ss >> temp)
//	{
//		timeSeparated.push_back(temp);
//	}
//	cout << endl;
//
//	return timeSeparated;
//}
//
//
////Separate, erase start, and every element from date: and after
////Then change it to an int
////Then print the string
////Note, I'll need to think about how to pad a 0
//void ClockClass::differenceBetweenStartAndStop(HDC hdc, string startTime, string stopTime)
//{
//	vector<int> start = stringSeparation(startTime);
//	vector<int> stop = stringSeparation(stopTime);
//	//start - stop
//	vector<int> totalTime;
//	int total = 0;
//	//loop through array and find the difference
//	for (unsigned int i = 0; i < start.size(); i++)
//	{
//		total = stop[i] - start[i];
//		totalTime.push_back(total);
//	}
//
//	RECT duration;
//	SetRect(&duration, 115, 225, 115, 175);
//
//	string test = "Duration: " + to_string(totalTime[0]) + ":" + to_string(totalTime[1]) + ":" + to_string(totalTime[2]);
//
//	DrawText(hdc, test.c_str(), -1, &duration, DT_LEFT | DT_NOCLIP);
//
//	//cout << endl;
//
//	//return totalTime;
//}