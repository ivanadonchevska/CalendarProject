#pragma once

#include <iostream>
#include <string>

using namespace std;

class Date {
public:
	int day;
	int month;
	int year;
	int weekDay; //return number of weekDay


	Date();

	//not sure why will need them when parameters are public but ok
	int getYear();
	int getMonth();
	int getDay();
	int getWeekDay();

	void setDay(int);
	void setMonth(int);
	void setYear(int);
	void setDate(int, int, int);

	static const string dayNames[8];
	static const string monthNames[12];
	static const string weekDays[7];
	static const string shortWeekDays[7];


	//string toString() const;

};