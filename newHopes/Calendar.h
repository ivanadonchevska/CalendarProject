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
	int index;
	Date currentDate;
	vector<Event> events;
	map<string, vector<pair<string, string>>> mp;
	string startingDay = "Sun";

	//is it possible to make it glogal somehow and change it
public:
	Calendar();
	
	string getStartingDay();
	void setStartingDay(string);

	void startMenu();
	//helpers for date validation when calling addEvent 
	bool doesEventExist(string);
	bool isDateCorrect(Date);
	bool isEndDateValid(Date, Date);
	//FIX THEM TO WORK OK!!!



	int FirstDayOfMonth(int, int);
	int getNumberOfDays(int, int);
	void setFirstDay();
	void showCalendar();


	//void showScedule(); //?
	void listEvents(); //list all events in file
	void addEvent(); //to add event to file
	void deleteEvent(); //delete event from file

	void changeStartingDay();

	//void specialRead();
	void sortFile();
	void readFromFileInsertToVector();

	//bool compare(Event&, Event&);
	//string YYYY_MM_DD(string);

};
