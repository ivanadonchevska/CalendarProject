#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include "date.h"

Date::Date() {
    time_t t = time(NULL); //or 0 instead of null
    struct tm* now = localtime(&t);
    //localtime_s(&now, &t);
    year = now->tm_year + 1900;
    month = now->tm_mon + 1;
    day = now->tm_mday;
    weekDay = now->tm_wday;
}

void Date::setDay(int d) {
    day = d;
}
void Date::setMonth(int m) {
    month = m;
}
void Date::setYear(int y) {
    year = y;
}
void Date::setDate(int d, int m, int y) {
    day = d;
    month = m;
    year = y;
}

int Date::getDay() {
    return day;
}

int Date::getMonth() {
    return month;
}

int Date::getYear() {
    return year;
}

int Date::getWeekDay() {
    return weekDay;
}

const string Date::dayNames[8] = {
    "Sun",
    "Mon",
    "Tue",
    "Wen",
    "Thu",
    "Fri",
    "Sat",
    "Sun"
};

const string Date::monthNames[12] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

const string Date::weekDays[7] = {
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};

const string Date::shortWeekDays[7] = {
    "Su",
    "Mo",
    "Tu",
    "We",
    "Th",
    "Fr",
    "Sa"
};

const int Date::monthDays[12] = {
        31, 28, 31, 30, 31, 30,
       31, 31, 30, 31, 30, 31
};

int Date::countLeapYears(vector<int> date) {
    int years = date[2];
    // Check if the current year needs to be
    //  considered for the count of leap years
    // or not
    if (date[1] <= 2)
        years--;
    // An year is a leap year if it 
    // is a multiple of 4,
    // multiple of 400 and not a 
     // multiple of 100.
    return years / 4 - years / 100 + years / 400;
}