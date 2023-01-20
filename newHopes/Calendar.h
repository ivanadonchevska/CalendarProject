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
* header file for Calendar class
*
*/
#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include "Event.h"
#include "Date.h"
using namespace std;

class Calendar {
private:
	Date currentDate;
	vector<Event> events;
	string startingDay = "Sun";
	map <int, set<string>> mp;
	map<int, int> storeEvents;

public:
	
	Calendar();
	
	string getStartingDay();
	void writeToFileStartingDay();
	vector<int> getNextDay(int, int, int);
	void getScheduledEvents(); //get number of events that are scheduled for current day or the day after

	void startMenu();

	//helper function for date validation when calling addEvent 
	bool doesEventExist(string);
	void addEvent(); //add event to file

	vector<int> fromStringToVector(string); 

	int DayOfMonth(int, int, int);
	int getNumberOfDays(int, int);
	int getDifference(vector<int>, vector<int>); //to find difference between two dates
	void setFirstDay(int, int, int);
	void showCalendar();
	int getLongestStringSize(vector<string>);

	bool isValidDate(string);
	bool isLeap(int);
	void showScedule(); //list all events in file
	void listEvents(); //shows events for entered month
	void deleteEvent(); //delete event from file
	void changeStartingDay(); //to change first day when printing calendar

	
};
