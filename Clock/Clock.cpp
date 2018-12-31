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