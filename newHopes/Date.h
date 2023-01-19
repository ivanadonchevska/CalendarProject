/**
*
* Solution to course project # 10
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Ivana Donchevska
* @idnumber 855364
* @compiler VC
*
* header file for Data class
*
*/
#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Date {
public:
	int day;
	int month;
	int year;
	int weekDay; //return number of weekDay

	Date();

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
	static const int monthDays[12];
	static int countLeapYears(vector<int>);

};