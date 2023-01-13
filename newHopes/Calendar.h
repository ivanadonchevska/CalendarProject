#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <unordered_map>
#include "Event.h"
#include "Date.h"
using namespace std;

class Calendar {
private:
	Date currentDate;
	vector<Event> events;
	string startingDay = "Sun";
	//is it possible to make it glogal somehow and change it
public:
	
	Calendar();
	
	string getStartingDay();
	void setStartingDay(string);

	void startMenu();

	//helpers for date validation when calling addEvent 
	bool doesEventExist(string);
	void addEvent(); //to add event to file

	int FirstDayOfMonth(int, int);
	int getNumberOfDays(int, int);
	void setFirstDay();
	void showCalendar();

	//void showScedule(); //?
	void listEvents(); //list all events in file
	void deleteEvent(); //delete event from file
	void changeStartingDay(); //fix it to work okey

};
