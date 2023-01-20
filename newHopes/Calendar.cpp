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
* file in which all the functions from Calendar.h are implemented
*
*/
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
#include "Calendar.h"

using namespace std;

const int DAYS_NUMBER = 7;
const int MAX_VALID_YR = 9999;
const int MIN_VALID_YR = 1800;

Calendar::Calendar() {}

string Calendar::getStartingDay() {
	return startingDay;
}

vector<int> Calendar::getNextDay(int day, int month, int year) {
	vector<int> nextDay;
	if (day > 0 && day < 28) {  // checking for day from 0 to 27
		day += 1;
	}
	if (day == 28) {
		if (month == 2) {  // checking for february
			if ((year % 400 == 0) || (year % 100 != 0 || year % 4 == 0)) {  // leap year check in case of february
				day = 29;
			}
			else {
				day = 1;
				month = 3;
			}
		}
		else {  // when it's not february
			day += 1;
		}
	}
	if (day == 29) {  // last day check for february
		if (month == 2) {
			day = 1;
			month = 3;
		}
		else {
			day += 1;
		}
	}
	if (day == 30) {  // last day check for april, june, september, november
		if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
			day += 1;
		}
		else {
			day = 1;
			month += 1;
		}
	}
	if (day == 31) {  // last day of the month
		day = 1;
		if (month == 12) {  // checking for last day of the year
			year += 1;
			month = 1;
		}
		else {
			month += 1;
		}
	}

	nextDay.push_back(day);
	nextDay.push_back(month);
	nextDay.push_back(year);

	return nextDay;
}

void Calendar::getScheduledEvents() {
	int day = currentDate.getDay();
	int month = currentDate.getMonth();
	int year = currentDate.getYear();
	vector<int> nextDay = getNextDay(day, month, year);
	int currentDayEvents = 0; //number of events for current day
	int nextDayEvents = 0; //number of events for the day after
	
	ifstream Read("Events.txt");
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

		if (IntStartDate[0] <= day && IntStartDate[1] <= month && IntStartDate[2] <= year &&
			day <= IntEndDate[0] && month <= IntEndDate[1] && year <= IntEndDate[2]) {
			currentDayEvents++;
		}
		else if (IntStartDate[0] <= nextDay[0] && IntStartDate[1] <= nextDay[1] && IntStartDate[2] <= nextDay[2] &&
			nextDay[0] <= IntEndDate[0] && nextDay[1] <= IntEndDate[1] && nextDay[2] <= IntEndDate[2]) {
			nextDayEvents++;
		}
		else
			continue;
	}
	Read.close();

	if (currentDayEvents != 0) {
		cout << "You have " << currentDayEvents;
		if (currentDayEvents == 1)
			cout << " event today. \n";
		else 
			cout << " events today. \n";
	}
	else if (currentDayEvents == 0 && nextDayEvents != 0) {
		cout << "You have " << nextDayEvents;
		if (nextDayEvents == 1)
			cout << " event tomorrow. \n";
		else
			cout << " events tomorrow. \n";
	}
	else
		cout << "There is no scheduled events for today and tomorrow. \n";
	
}
void Calendar::writeToFileStartingDay() {
	//write to file default startingDay
	ofstream Write;
	Write.open("startingDate.txt", ofstream::app);
	Write << startingDay;
	Write.close();
}

void Calendar::startMenu() {
	int wDay = currentDate.getWeekDay();
	int day = currentDate.getDay();
	int month = currentDate.getMonth();
	int year = currentDate.getYear();

	cout << "Welcome! Today is " << Date::weekDays[wDay - 1] << ", " << day << " " << Date::monthNames[month - 1] << " " << year << "." << endl;
	cout << endl;
	getScheduledEvents();
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
	//to check if choice is integer or not
	while (!cin.good())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	cout << endl;

	switch (choice) {
	case 1:
		showCalendar(); 
		break;
	case 2:
		showScedule(); 
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


bool Calendar::doesEventExist(string eventToFind) {
	string line;
	ifstream Read;
	Read.open("Events.txt");

	while (getline(Read, line)) {
		if (line.find(eventToFind) != string::npos) {
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

bool Calendar::isLeap(int year) {
	return (((year % 4 == 0) &&
		(year % 100 != 0)) ||
		(year % 400 == 0));
}
bool Calendar::isValidDate(string date) {
	string ch1 = date;
	string ch2 = date;
	ch1 = ch1.erase(0, 2);
	ch1 = ch1.substr(0, 1);
	ch2 = ch2.erase(0, 5);
	ch2 = ch2.substr(0, 1);

	if (ch1 != "/" || ch2 != "/")
		return false;
	
	vector<int> vectorStartDate;
	vectorStartDate = fromStringToVector(date);

	if (vectorStartDate[2] > MAX_VALID_YR ||
		vectorStartDate[2] < MIN_VALID_YR)
		return false;
	if (vectorStartDate[1] < 1 || vectorStartDate[1] > 12)
		return false;
	if (vectorStartDate[0] < 1 || vectorStartDate[0] > 31)
		return false;

	if (vectorStartDate[1] == 2)
	{
		if (isLeap(vectorStartDate[2]))
			return (vectorStartDate[0] <= 29);
		else
			return (vectorStartDate[0] <= 28);
	}

	if (vectorStartDate[1] == 4 || vectorStartDate[1] == 6 ||
		vectorStartDate[1] == 9 || vectorStartDate[1] == 11)
		return (vectorStartDate[0] <= 30);

	return true;
}

void Calendar::addEvent() {
	ofstream Write;
	Write.open("Events.txt", ofstream::app);
	Event event;

	cout << "Enter name: ";
	int counter = 0;
	do {
		if(counter != 0)
			cout << "This event name already exist! Try another one. \n";
		cin >> ws;
		getline(cin, event.eventName);
		counter++;
	} while (doesEventExist(event.eventName));
	Write << event.eventName << "-";
	
	string currentDateString;
	if (currentDate.day <= 9)
		currentDateString += "0" + to_string(currentDate.day) + "/";
	else
		currentDateString += to_string(currentDate.day) + "/";
	if (currentDate.month <= 9)
		currentDateString += "0" + to_string(currentDate.month) + "/";
	else
		currentDateString += to_string(currentDate.month) + "/";
	currentDateString += to_string(currentDate.year);
	
	cout << "Enter start date (DD/MM/YYYY): ";
	cin >> event.startDate;
	
	while (!isValidDate(event.startDate)){
		cout << "Entered start date is not valid! Try again. \n";
		cin >> event.startDate;
	}
	
	while (YYYY_MM_DD(event.startDate) < YYYY_MM_DD(currentDateString)) {
		cout << "Past events can not be added. Try again! \n";
		cin >> event.startDate;
	}
	Write << event.startDate << "-";

	cout << "Enter end date (DD/MM/YYYY): ";
	cin >> event.endDate;

	while (!isValidDate(event.endDate)) {
		cout << "Entered end date is not valid! Try again. \n";
		cin >> event.endDate;
	}
	
	while (YYYY_MM_DD(event.endDate) < YYYY_MM_DD(event.startDate)) {
		cout << "End date can not be before start date. Try again! \n";
		cin >> event.endDate;
	}
	Write << event.endDate << "\n";
	
	Write.close();
	cout << "Event added successfully!";
}

void Calendar::showScedule() {
	ifstream Read("Events.txt");
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

	if (events.size() != 0) {
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
	}
	else {
		cout << "Don't have following events yet! \n";
	}
};

void Calendar::deleteEvent() {
	string deleteEvent;
	string line;

	ifstream Delete;
	Delete.open("Events.txt");
	ofstream temp;
	temp.open("temp.txt");
	cout << "Enter name: "; //to delete line that contains inserted event name
	cin >> ws;
	getline(cin, deleteEvent);

	if (doesEventExist(deleteEvent) == false) {
		cout << "Can't delete event, that not exist! \n";
	}
	else {
		while (getline(Delete, line)) {
			string id(line.begin(), line.begin() + line.find("-"));
			if (id != deleteEvent)
				temp << line << endl;
		}
		cout << "Event deleted successfully!";
		temp.close();
		Delete.close();
		remove("Events.txt");
		rename("temp.txt", "Events.txt");
	}
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
	//months which have 31 days
	else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8
		|| month == 10 || month == 12)
		return 31;
	else
		return 30;
}

//using fromStringToVector for input data
int Calendar::getDifference(vector<int> date1, vector<int> date2) {
	long int counter1 = date1[2] * 365 + date1[0];
	for (int i = 0; i < date1[1] - 1; i++)
		counter1 += Date::monthDays[i];
	counter1 += Date::countLeapYears(date1);

	long int counter2 = date2[2] * 365 + date2[0];
	for (int i = 0; i < date2[1] - 1; i++)
		counter2 += Date::monthDays[i];
	counter2 += Date::countLeapYears(date2);

	return (counter2 - counter1) + 1;
}

void Calendar::setFirstDay(int month, int year, int longest) {
	string currentStartingDay;
	ifstream Read("StartingDate.txt");
	while (getline(Read, currentStartingDay)) {
	}

	int currentDay = 0;
	int counter = 0;
	if (currentDate.getMonth() == month && currentDate.getYear() == year) {
		currentDay = currentDate.getWeekDay();
	}
	if (currentStartingDay == "Sun") {
		for (int i = 0; i < DAYS_NUMBER; i++) {
			if (i == currentDay && currentDate.getMonth() == month && currentDate.getYear() == year) {
				cout << "[" << Date::dayNames[i] << "]" << " ";
				counter = 5; //3 for letters and +2 for []
				if (counter > longest) {
					cout << " ";
				}
				else {
					while (counter != longest) {
						cout << " ";
						counter++;
					}
				}
			}
			else {
				cout << Date::dayNames[i] << " ";
				counter = 3; //3 for letters
				if (counter > longest) {
					cout << " ";
				}
				else {
					while (counter != longest) {
						cout << " ";
						counter++;
					}
				}
			}
		}
	}
	else if (currentStartingDay == "Mon") {
		for (int i = 1; i < DAYS_NUMBER + 1; i++) {
			if (i == currentDay && currentDate.getMonth() == month && currentDate.getYear() == year) {
				cout << "[" << Date::dayNames[i] << "]" << " ";
				counter = 5; //3 for letters and +2 for []
				if (counter > longest) {
					cout << " ";
				}
				else {
					while (counter != longest) {
						cout << " ";
						counter++;
					}
				}
			}
			else {
				cout << Date::dayNames[i] << " ";
				counter = 3; //3 for letters
				if (counter > longest) {
					cout << " ";
				}
				else {
					while (counter != longest) {
						cout << " ";
						counter++;
					}
				}
			}
		}
	}

	Read.close();
}

int Calendar::getLongestStringSize(vector<string> printVector) {
	int longest = 0;
	for (auto& element : printVector) {
		if (element.size() > longest)
			longest = element.size();
	}
	return longest;
}

void Calendar::showCalendar() {
	int year = 0; 
	int month = 0; 
	char ch;
	vector<int> insertedStartDate; 
	insertedStartDate.push_back(1);
	insertedStartDate.push_back(month);
	insertedStartDate.push_back(year);
	vector<int> insertedEndDate;
	insertedEndDate.push_back(getNumberOfDays(month, year));
	insertedEndDate.push_back(month);
	insertedEndDate.push_back(year);

	cout << "Enter month (MM/YYYY): ";

	cin >> month >> ch >> year;

	while (month < 0 || month > 12 || ch != '/' || year < MIN_VALID_YR || year > MAX_VALID_YR) {
		cout << "Inserted invalid data! Try again.\n";
		cin >> month >> ch >> year;
	}

	cout << endl;
	cout << "        " << Date::monthNames[month - 1];
	cout << "    " << year << endl;

	//read the file and insert to map needed data
	ifstream Read("Events.txt");
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
				while (startFrom <= IntEndDate[0] || startFrom <= getNumberOfDays(IntStartDate[1], IntStartDate[2])) {
					storeEvents[startFrom]++;
					startFrom++;
				}
			}
		}
		else if (month > IntStartDate[1] && year >= IntStartDate[2] && month <= IntEndDate[1] && year <= IntEndDate[2]) {
			int startFrom = 1; //start from first day of month
			if (month == IntEndDate[1]) { //in case when inserted month is the same as endMonth
				while (startFrom <= IntEndDate[0] && startFrom <= getNumberOfDays(IntStartDate[1], IntStartDate[2])) {
					storeEvents[startFrom]++;
					startFrom++;
				}
			}
			else { //case when month is full and endDay is not in it
				while (startFrom <= IntEndDate[0] || startFrom <= getNumberOfDays(IntStartDate[1], IntStartDate[2])) {
					storeEvents[startFrom]++;
					startFrom++;
				}
			}
		}
		else
			continue;
	}
	Read.close();

	string start;
	string dayFromFile;
	ifstream ReadStartingDay("StartingDate.txt");
	while (getline(ReadStartingDay, dayFromFile)) {
		start = dayFromFile;
	}
	ReadStartingDay.close();

	//  Tomohiko Sakamoto's Algorithm for DayOfMonth
	int day = 1;
	int startDay = 0;
	if (start == "Sun")
		startDay = DayOfMonth(day, month, year);
	else if (start == "Mon") {
		startDay = DayOfMonth(day, month, year);
		if (startDay == 0) {
			startDay += 6;
		}
		else {
			startDay -= 1;
		}
	}
	
	int dayCount = getNumberOfDays(month, year);

	vector<string> printVector;
	string currentPrint;

	//Add the date to vector corresponding with the day 
	for (int d = 0; d < dayCount; ++d) {
		if (d < 9) {
			//current day plus has events
			if (storeEvents.find(d + 1) != storeEvents.end() && currentDate.getDay() == d + 1 && currentDate.getMonth() == month && currentDate.getYear() == year) {
				for (auto& x : storeEvents) {
					if (x.first == d + 1) {
						currentPrint = "[0" + to_string(d + 1) + "](" + to_string(x.second) + ")";
						printVector.push_back(currentPrint);
					}
				}
			}
			//has events and not current day
			else if (storeEvents.find(d + 1) != storeEvents.end() ) {
				for (auto& x : storeEvents) {
					if (x.first == d + 1) {
						currentPrint = "0" + to_string(d + 1) + "(" + to_string(x.second) + ")";
						printVector.push_back(currentPrint);
					}
				}
			}
			//current day but without events
			else if (storeEvents.find(d + 1) == storeEvents.end() && currentDate.getDay() == d + 1 && currentDate.getMonth() == month && currentDate.getYear() == year) {
				currentPrint = "[0" + to_string(d + 1) + "]";
				printVector.push_back(currentPrint);
			}
			else {
				currentPrint = "0" + to_string(d + 1);
				printVector.push_back(currentPrint);
			}
		}
		else {
			if (storeEvents.find(d + 1) != storeEvents.end() && currentDate.getDay() == d + 1 && currentDate.getMonth() == month && currentDate.getYear() == year) {
				for (auto& x : storeEvents) {
					if (x.first == d + 1) {
						currentPrint = "[" + to_string(d + 1) + "](" + to_string(x.second) + ")";
						printVector.push_back(currentPrint);
					}
				}
			}
			else if (storeEvents.find(d + 1) != storeEvents.end()) {
				for (auto& x : storeEvents) {
					if (x.first == d + 1) {
						currentPrint = to_string(d + 1) + "(" + to_string(x.second) + ")";
						printVector.push_back(currentPrint);
					}
				}
			}
			else if (storeEvents.find(d + 1) == storeEvents.end() && currentDate.getDay() == d + 1 && currentDate.getMonth() == month && currentDate.getYear() == year) {
				currentPrint = "[" + to_string(d + 1) + "]";
				printVector.push_back(currentPrint);
			}
				
			else {
				currentPrint = to_string(d + 1);
				printVector.push_back(currentPrint);
			}
	
		}
		
	}

	int longest = getLongestStringSize(printVector);
	setFirstDay(month, year, longest);
	cout << endl;

	//Print an empty space if the day doesn't have a corresponding day 
	if (longest != 2) {
		for (int x = 0; x < startDay; ++x) {
			std::cout << "      ";
		}
	}
	else {
		for (int x = 0; x < startDay; ++x) {
			std::cout << "     ";
		}
	}

	for (auto& x : printVector) {
		if (longest == 2) {
			cout << x << "   ";
		}
		else if(x.size() < longest){
			cout << x << " ";
			int counter = x.size();
			while (counter < longest) { 
				cout << " ";
				counter++;
			}
		}
		else
			cout << x << " ";
		startDay++;
		if (startDay == 7) {
			startDay = 0;
			cout << endl;
		}
	}

	cout << endl;
}

void Calendar::changeStartingDay() {
	string currentDay;
	string changeDay;
	string wholeDayName;
	ifstream Read("StartingDate.txt");
	ofstream Temp("TempStartingDay.txt");
	while (getline(Read, currentDay)) {
		if (currentDay == "Sun")
			wholeDayName = "Sunday";
		else if (currentDay == "Mon")
			wholeDayName = "Monday";
	}
	
	cout << "The first day of the week is currently " << wholeDayName << "." << endl;
	cout << endl;
	cout << "Enter new(Mon / Sun): ";
	int counter = 0;
	do {
		if (counter != 0)
			cout << "You can only choose between Mon and Sun for starting day! Try again. \n";
		cin >> changeDay;
		counter++;
	} while (changeDay != "Mon" && changeDay != "Sun");

	if (currentDay != changeDay) {
		Temp << changeDay;
	}

	Temp.close();
	Read.close();
	remove("StartingDate.txt");
	rename("TempStartingDay.txt", "StartingDate.txt");
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

	string testingDate;
	testingDate = "01/" + monthAndYear;
	while (!isValidDate(testingDate)) {
		cout << "Incorrect input! Try again. \n";
		cin >> monthAndYear;
		testingDate = "01/" + monthAndYear;
	}

	string month = monthAndYear.substr(0, 2); 
	int IntMonth = stoi(month); //stoi is used to convert string to integer
	string year = monthAndYear.substr(3, 6);
	int IntYear = stoi(year);
	vector<int> insertedDate; //to have current date as vector, so can use it if current month is not startMonth but also has events
	insertedDate.push_back(1); //to start from first day of the month
	insertedDate.push_back(IntMonth);
	insertedDate.push_back(IntYear);

	cout << "\n";
	cout << Date::monthNames[IntMonth - 1] << " " << year << "\n";
	cout << "------------\n"; 

	ifstream Read("Events.txt"); 
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
				numOfEvents = getDifference(IntStartDate, IntEndDate);
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
		//if not inserted month but between start and end date it also includes events
		else if (IntMonth > IntStartDate[1] && IntYear >= IntStartDate[2] && IntMonth <= IntEndDate[1] && IntYear <= IntEndDate[2]) {
			int startFrom = 1; //start from first day of month
			int numOfPassedEvents = getDifference(IntStartDate, insertedDate); //to count how many events are passed and from which one to start
			int numOfEvents = 0;
			numOfEvents = getDifference(IntStartDate, IntEndDate);
			while (numOfPassedEvents <= numOfEvents && startFrom < getNumberOfDays(IntStartDate[1], IntStartDate[2])) {
				string fullEventName = event.eventName + " (Day " + to_string(numOfPassedEvents) + "/" + to_string(numOfEvents) + ")";
				mp[startFrom].insert(fullEventName);
				startFrom++;
				numOfPassedEvents++;
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