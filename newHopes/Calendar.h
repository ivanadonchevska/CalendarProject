#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <unordered_map>
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
	
	//is it possible to make it glogal somehow and change it
public:
	
	Calendar();
	
	string getStartingDay();
	void writeToFileStartingDay();

	vector<int> getNextDay(int, int, int);
	void getScheduledEvents(); //get number of events that are scheduled for current day or the day after
	void startMenu();

	//helpers for date validation when calling addEvent 
	bool doesEventExist(string);
	void addEvent(); //to add event to file

	vector<int> fromStringToVector(string);

	int DayOfMonth(int, int, int);
	int getNumberOfDays(int, int);
	int getDifference(vector<int>, vector<int>); //to find difference between two dates
	void setFirstDay(int, int, int);
	void showCalendar();
	int getLongestStringSize(vector<string>);

	void showScedule(); //?
	void listEvents(); //list all events in file
	void deleteEvent(); //delete event from file
	void changeStartingDay();

	
};
