#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include "date.h"

Date::Date() {
    time_t t = time(NULL); //or 0 insead of null
    struct tm now;
    localtime_s(&now, &t);
    year = now.tm_year + 1900;
    month = now.tm_mon + 1;
    day = now.tm_mday;
    weekDay = now.tm_wday;
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
