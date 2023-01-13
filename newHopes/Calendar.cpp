#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>


#include "Calendar.h"
using namespace std;

const int DAYS_NUMBER = 7;


Calendar::Calendar() {}

string Calendar::getStartingDay() {
	return startingDay;
}
void Calendar::setStartingDay(string newStartingDay) {
	startingDay = newStartingDay;
}

void Calendar::startMenu() {
	int wDay = currentDate.getWeekDay();
	int day = currentDate.getDay();
	int month = currentDate.getMonth();
	int year = currentDate.getYear();

	cout << "Welcome! Today is " << Date::weekDays[wDay - 1] << ", " << day << " " << Date::monthNames[month - 1] << " " << year << "." << endl;
	cout << endl;
	cout << "Choose an option:" << endl;
	cout << "     1. Show calendar" << endl;
	cout << "     2. Show schedule" << endl;
	cout << "     3. List events" << endl;
	cout << "     4. Add event" << endl;
	cout << "     5. Remove event" << endl;
	cout << "     6. Set first weekday" << endl;
	cout << endl;

	int choice;
	cout << "Enter your choice: ";
	cin >> choice;

	switch (choice) {
	case 1:
		showCalendar(); //staft from Sunday by default
		break;
	case 2:
		//showScedule(); //do it!!
		break;
	case 3:
		Calendar::listEvents();
		break;
	case 4:
		addEvent();
		break;
	case 5:
		deleteEvent();
		break;
	case 6:
		changeStartingDay();
		break;
	default:
		cout << "Invalid input." << endl;
		break;
	}
}

bool Calendar::doesEventExist(string eventToFind) {
	string line;
	ifstream Read;
	Read.open("newText4.txt");

	while (getline(Read, line)) {
		if (line.find(eventToFind) != string::npos) {
			cout << "This event name already exist! Try another one. \n";
			return true;
		}
	}
	Read.close();
	return false;
	
}

//function to convert DD/MM/YYYY to YYYY/MM/DD to be easy to compare
string YYYY_MM_DD(string dateTime) {
	string s1 = dateTime;
	string day = s1.substr(0, 2);
	string year = s1.substr(6, 4);

	s1.erase(0, 2);
	s1.erase(s1.size() - 4);
	//cout << s1 << endl;
	s1.insert(0, year);
	s1.insert(8, day);
	//cout << s1;

	return s1;
}

bool compare(Event& lhs, Event& rhs) {
	if (YYYY_MM_DD(lhs.startDate) < YYYY_MM_DD(rhs.startDate))
		return true;
	if (YYYY_MM_DD(lhs.startDate) == YYYY_MM_DD(rhs.startDate) && lhs.eventName < rhs.eventName)
		return true;
	//if (lhs.startYear == rhs.startYear && lhs.startMonth == rhs.startMonth && lhs.startDay < rhs.startDay)
		//return true;
	//if (lhs.startYear == rhs.startYear && lhs.startMonth == rhs.startMonth && lhs.startDay == rhs.startDay && lhs.eventName < rhs.eventName)
		//return true;
	return false;
}

void Calendar::addEvent() {
	ofstream Write;
	Write.open("newText4.txt", ofstream::app);
	Event event;

	cout << "Enter name: ";
	do {
		getline(cin, event.eventName);
	} while (doesEventExist(event.eventName));
	Write << event.eventName << " ";

	
	cout << "Enter start date (DD/MM/YYYY): ";
	cin >> event.startDate;
	Write << event.startDate << " ";
	cout << "Enter end date (DD/MM/YYYY): ";
	cin >> event.endDate;
	while (event.endDate < event.startDate) {
		cout << "End date can not be before start date. Try again! \n";
		cin >> event.endDate;
	}
	Write << event.endDate << "\n";
	
	Write.close();
	cout << "Event added successfully!";
}

//make comment if there is no events!!!
void Calendar::listEvents() {
	ifstream Read("newText4.txt");
	string line;
	
	while (getline(Read, line)) {
		istringstream iss(line);
		string substr;
		vector<string> substrs;

		while (getline(iss, substr, ' ')) {
			substrs.push_back(substr);
		}
		Event event;
		event.eventName = substrs[0];
		event.startDate = substrs[1];
		event.endDate = substrs[2];

		events.push_back(event);
	}
	Read.close();

	sort(events.begin(), events.end(), compare);

	cout << "You have the following events: \n";
	for (int i = 0; i < events.size(); i++) {
		cout << i + 1 << ". " << events[i].eventName << " (" << events[i].startDate;
		if (events[i].startDate == events[i].endDate) {
			cout << ")" << "\n";
		}
		else {
			cout << " - " << events[i].endDate << ") \n";
		}
	}
	/*
	ofstream Temp;
	Temp.open("TempFile.txt");
	
	for (int i = 0; i < events.size(); i++) {
		Temp << events[i].eventName << " (" << events[i].startDate;
		if (events[i].startDate == events[i].endDate) {
			Temp << ")" << "/n";
		}
		else {
			Temp << " - " << events[i].endDate << ") /n";
		}
	}

	Temp.close();
	
	remove("newText.txt");
	rename("TempFile.txt", "newText2.txt");
	*/
};
/*
//if found solution to do it better use this function for print events
void Calendar::listEvents() {
	string line;
	ifstream Read("newText3.txt");

	cout << "You have the following events:" << endl;
	if (Read.is_open()) {
		int i = 1;
		while (getline(Read, line)) {
			cout << i << ". " << line << "\n";
			i++;
		}
		Read.close();
	}

	else
		cout << "Unable to open file";
}
*/
void Calendar::deleteEvent() {
	string deleteEvent;
	string line;

	ifstream Delete;
	Delete.open("newText4.txt");
	ofstream temp;
	temp.open("temp.txt");
	cout << "Enter name: "; //input line to remove
	cin >> deleteEvent;

	while (getline(Delete, line))
	{
		string id(line.begin(), line.begin() + line.find(" "));
		if (id != deleteEvent)
			temp << line << endl;
	}

	temp.close();
	Delete.close();
	remove("newText4.txt");
	rename("temp.txt", "newText4.txt");

	cout << "Event deleted successfully!";
}

int Calendar::FirstDayOfMonth(int year, int month)
{
	int d = 1;
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	year -= month < 3;
	return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + d) % 7;
}

int Calendar::getNumberOfDays(int month, int year) {
	//leap year condition, if month is 2
	if (month == 2)
	{
		if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
			return 29;
		else
			return 28;
	}
	//months which has 31 days
	else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8
		|| month == 10 || month == 12)
		return 31;
	else
		return 30;
}

void Calendar::setFirstDay() {
	if (startingDay == "Sun") {
		for (int i = 0; i < DAYS_NUMBER; i++) {
			cout << Date::dayNames[i] << " ";
		}
	}
	else if (startingDay == "Mon") {
		for (int i = 1; i < DAYS_NUMBER + 1; i++) {
			cout << Date::dayNames[i] << " ";
		}
	}
}

//fix it it's not okey
void Calendar::showCalendar() {
	int year; // = currentDate.getYear();
	int month; //= currentDate.getMonth();
	char ch;
	cout << "Enter month (MM/YYYY): ";

	cin >> month >> ch >> year;
	cout << endl;
	cout << "    " << Date::monthNames[month - 1];
	cout << "    " << year << endl;

	setFirstDay();

	cout << endl;

	//  Tomohiko Sakamoto's Algorithm

	int startDay = 0;
	if (startingDay == "Sun")
		startDay = FirstDayOfMonth(year, month);
	else if (startingDay == "Mon") {
		startDay = FirstDayOfMonth(year, month);
		if (startDay == 0) {
			startDay += 6;
		}
		else {
			startDay -= 1;
		}
	}
	int dayCount = getNumberOfDays(month, year);


	//Print and empty space if the day doesn't have a corresponding day 
	for (int x = 0; x < startDay; ++x) {
		cout << "    ";
	}


	//Print the date corresponding with the day 
	for (int d = 0; d < dayCount; ++d) {
		if (d < 9)

			cout << "0" << d + 1 << "  ";
		else
			cout << d + 1 << "  ";
		startDay++;
		if (startDay == 7) {
			startDay = 0;
			cout << endl;
		}
	}

	cout << endl;
}

//if called this function should change starting day when call print calendar??
//not sure how to do it ^_-
void Calendar::changeStartingDay() {
	string changeDay;
	string wholeDayName;
	if (getStartingDay() == "Sun")
		wholeDayName = "Sunday";
	else if (getStartingDay() == "Mon")
		wholeDayName = "Monday";
	else
		cout << "Invalid input" << endl;

	cout << "The first day of the week is currently " << wholeDayName << "." << endl;
	cout << endl;
	cout << "Enter new(Mon / Sun): ";
	cin >> changeDay;
	setStartingDay(changeDay);
	//startingDay = changeDay;
	cout << endl;
	cout << "Saved!";
}

