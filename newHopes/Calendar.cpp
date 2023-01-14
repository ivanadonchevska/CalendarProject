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
		showScedule(); //do it!!
		break;
	case 3:
		listEvents();
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

//fix it not to print this message when call from start menu
bool Calendar::doesEventExist(string eventToFind) {
	string line;
	ifstream Read;
	Read.open("newText5.txt");

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
	s1.insert(0, year);
	s1.insert(8, day);

	return s1;
}

bool compare(Event& lhs, Event& rhs) {
	if (YYYY_MM_DD(lhs.startDate) < YYYY_MM_DD(rhs.startDate))
		return true;
	if (YYYY_MM_DD(lhs.startDate) == YYYY_MM_DD(rhs.startDate) && lhs.eventName < rhs.eventName)
		return true;
	return false;
}

void Calendar::addEvent() {
	ofstream Write;
	Write.open("newText5.txt", ofstream::app);
	Event event;

	cout << "Enter name: ";
	do {
		getline(cin, event.eventName);
	} while (doesEventExist(event.eventName));
	Write << event.eventName << "-";
	
	cout << "Enter start date (DD/MM/YYYY): ";
	cin >> event.startDate;
	Write << event.startDate << "-";

	cout << "Enter end date (DD/MM/YYYY): ";
	cin >> event.endDate;
	
	while (YYYY_MM_DD(event.endDate) < YYYY_MM_DD(event.startDate)) {
		cout << "End date can not be before start date. Try again! \n";
		cin >> event.endDate;
	}
	Write << event.endDate << "\n";
	
	Write.close();
	cout << "Event added successfully!";
}

//make comment if there is no events!!!
void Calendar::showScedule() {
	ifstream Read("newText5.txt");
	string line;
	
	while (getline(Read, line)) {
		istringstream iss(line);
		string substr;
		vector<string> substrs;

		while (getline(iss, substr, '-')) {
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
};

void Calendar::deleteEvent() {
	string deleteEvent;
	string line;

	ifstream Delete;
	Delete.open("newText5.txt");
	ofstream temp;
	temp.open("temp.txt");
	cout << "Enter name: "; //input line to remove
	getline(cin, deleteEvent);

	while (getline(Delete, line)){
		string id(line.begin(), line.begin() + line.find("-"));
		if (id != deleteEvent)
			temp << line << endl;
	}

	temp.close();
	Delete.close();
	remove("newText5.txt");
	rename("temp.txt", "newText5.txt");

	cout << "Event deleted successfully!";
}

int Calendar::DayOfMonth(int day, int month, int year){
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	year -= month < 3;
	return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
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

void Calendar::setFirstDay(int month, int year) {
	int currentDay = 0;
	if (currentDate.getMonth() == month && currentDate.getYear() == year) {
		currentDay = currentDate.getWeekDay();
	}
	if (startingDay == "Sun") {
		for (int i = 0; i < DAYS_NUMBER; i++) {
			if (i == currentDay && currentDate.getMonth() == month && currentDate.getYear() == year)
				cout << "[" << Date::dayNames[i] << "]" << "   ";
			else 
				cout << Date::dayNames[i] << "   ";
		}
	}
	else if (startingDay == "Mon") {
		for (int i = 1; i < DAYS_NUMBER + 1; i++) {
			if (i == currentDay  && currentDate.getMonth() == month && currentDate.getYear() == year)
				cout << "[" << Date::dayNames[i] << "]" << "   ";
			else 
				cout << Date::dayNames[i] << "   ";
		}
	}
}

//fix it it's not okey
void Calendar::showCalendar() {
	int year; // = currentDate.getYear();
	int month; //= currentDate.getMonth();
	char ch;
	//cout << currentDate.getDay() << endl;
	//cout << currentDate.getMonth() << endl;
	cout << "Enter month (MM/YYYY): ";

	cin >> month >> ch >> year;
	cout << endl;
	cout << "    " << Date::monthNames[month - 1];
	cout << "    " << year << endl;

	setFirstDay(month, year);

	cout << endl;
	//read the file and insert to map needed data
	ifstream Read("newText5.txt");
	string line;

	while (getline(Read, line)) {
		istringstream iss(line);
		string substr;
		vector<string> substrs;

		while (getline(iss, substr, '-')) {
			substrs.push_back(substr);
		}
		Event event;
		event.eventName = substrs[0];
		event.startDate = substrs[1];
		event.endDate = substrs[2];

		vector<int> IntStartDate;
		IntStartDate = fromStringToVector(event.startDate);
		vector<int> IntEndDate;
		IntEndDate = fromStringToVector(event.endDate);

		if (month == IntStartDate[1] && year == IntStartDate[2]) {
			if(event.startDate == event.endDate) //add only one event to this data, because it starts and ends on the same date
				storeEvents[IntStartDate[0]]++;
			else {
				int startFrom = IntStartDate[0];
				int counter = 1;
				int numOfEvents = 0; 
				numOfEvents = IntEndDate[0] - IntStartDate[0] + 1;
				while (startFrom <= IntEndDate[0] && startFrom <= getNumberOfDays(IntStartDate[1], IntStartDate[2])) {
					storeEvents[startFrom]++;
					startFrom++;
					counter++;
				}
			}
		}
		else
			continue;
	}
	Read.close();

	//  Tomohiko Sakamoto's Algorithm
	int day = 1;
	int startDay = 0;
	if (startingDay == "Sun")
		startDay = DayOfMonth(day, month, year);
	else if (startingDay == "Mon") {
		startDay = DayOfMonth(day, month, year);
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
		std::cout << "      ";
	}

	//Print the date corresponding with the day 
	for (int d = 0; d < dayCount; ++d) {
		if (d < 9) {
			//current day plus has events
			if (storeEvents.find(d + 1) != storeEvents.end() && currentDate.getDay() == d + 1 && currentDate.getMonth() == month && currentDate.getYear() == year) {
				for (auto& x : storeEvents) {
					if (x.first == d + 1)
						cout << " [0" << d + 1 << "](" << x.second << ")";
				}
			}
			//has events and not current day
			else if (storeEvents.find(d + 1) != storeEvents.end() ) {
				for (auto& x : storeEvents) {
					if (x.first == d + 1)
						cout << " 0" << d + 1 << "(" << x.second << ") ";
				}
			}
			//current day but without events
			else if(storeEvents.find(d + 1) == storeEvents.end() && currentDate.getDay() == d + 1 && currentDate.getMonth() == month && currentDate.getYear() == year)
				cout << " [0" << d + 1 << "]  ";
			else
				cout << " 0" << d + 1 << "   ";
		}
		else {
			if (storeEvents.find(d + 1) != storeEvents.end() && currentDate.getDay() == d + 1 && currentDate.getMonth() == month && currentDate.getYear() == year) {
				for (auto& x : storeEvents) {
					if (x.first == d + 1)
						cout << " [" << d + 1 << "](" << x.second << ")";
				}
			}
			else if (storeEvents.find(d + 1) != storeEvents.end()) {
				for (auto& x : storeEvents) {
					if (x.first == d + 1)
						cout << d + 1 << "(" << x.second << ") ";
				}
			}
			else if(storeEvents.find(d + 1) == storeEvents.end() && currentDate.getDay() == d + 1 && currentDate.getMonth() == month && currentDate.getYear() == year)
				cout <<" [" << d + 1 << "]  ";
			else
				cout << " " << d + 1 << "   ";
	
		}
		startDay++;
		if (startDay == 7) {
			startDay = 0;
			cout << endl;
		}
	}

	cout << endl;

	//for (auto& x : storeEvents) {
	//	cout << x.first << " " << x.second << endl;
	//}
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

	cout << "The first day of the week is currently " << wholeDayName << "." << endl;
	cout << endl;
	cout << "Enter new(Mon / Sun): ";
	cin >> changeDay;
	setStartingDay(changeDay);
	cout << endl;
	cout << "Saved!";
}

vector<int> Calendar::fromStringToVector(string date) {
	vector<int> dateValues;
	string day = date;
	day.erase(2, 8);
	int IntDay = stoi(day);
	string month = date;
	month.erase(0, 3);
	month.erase(2, 6);
	int IntMonth = stoi(month);
	string year = date;
	year.erase(0, 6);
	int IntYear = stoi(year);

	dateValues.push_back(IntDay);
	dateValues.push_back(IntMonth);
	dateValues.push_back(IntYear);
	
	return dateValues;
}

void Calendar::listEvents() {
	string monthAndYear;
	cout << "\n";
	cout << "Enter month (MM/YYYY): ";
	cin >> monthAndYear;

	string month = monthAndYear.substr(0, 2); 
	int IntMonth = stoi(month); //stoi is used to convert string to integer
	string year = monthAndYear.substr(3, 6);
	int IntYear = stoi(year);
	cout << "\n";
	cout << Date::monthNames[IntMonth - 1] << " " << year << "\n";
	cout << "------------\n"; //if possible make it long as string month + year

	ifstream Read("newText5.txt"); //newText5.txt
	string line;

	if (events.size() != 0)
		events.clear();

	while (getline(Read, line)) {
		istringstream iss(line);
		string substr;
		vector<string> substrs;
		
		while (getline(iss, substr, '-')) {
			substrs.push_back(substr);
		}
		Event event;
		event.eventName = substrs[0];
		event.startDate = substrs[1];
		event.endDate = substrs[2];

		vector<int> IntStartDate;
		IntStartDate = fromStringToVector(event.startDate);
		vector<int> IntEndDate;
		IntEndDate = fromStringToVector(event.endDate);

		if (IntStartDate[1] == IntMonth) { //insert in map only events that start on inserted month
			if (event.startDate == event.endDate) {
				mp[IntStartDate[0]].insert(event.eventName);
			}
			else {
				int startFrom = IntStartDate[0];
				int counter = 1;
				int numOfEvents = 0; //this variable is to store difference between first and last day of given event, so we can count how many events are
				if (IntEndDate[0] > IntStartDate[0])
					numOfEvents = IntEndDate[0] - IntStartDate[0] + 1;
				else {
					numOfEvents = (getNumberOfDays(IntStartDate[1], IntStartDate[2]) - IntStartDate[0]) + IntEndDate[0] + 1;
				}
				//&& startFrom <= getNumberOfDays(IntStartDate[1], IntStartDate[2]) + 1
				int eventsCnt = 0;
				while (eventsCnt < numOfEvents && startFrom <= getNumberOfDays(IntStartDate[1], IntStartDate[2])) {
					string fullEventName = event.eventName + " (Day " + to_string(counter) + "/" + to_string(numOfEvents) + ")";
					mp[startFrom].insert(fullEventName);
					startFrom++;
					counter++;
					eventsCnt++;
				}
			}
		}
		else
			continue;
	
	}
	Read.close();
	
	for (auto& x : mp) {	
		int startWeekDay = DayOfMonth(x.first, IntMonth, IntYear);
		int counter = 0; 
		cout << Date::shortWeekDays[startWeekDay] << ", " << x.first << "\t";
		for (auto& y : x.second) {
			cout << y << "\n";
			if (x.second.size() > 1 && counter < x.second.size() - 1) { 
				cout << "        ";
				counter++;
			}
		}
		

	}
	
}
