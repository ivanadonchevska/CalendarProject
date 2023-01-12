#include <iostream>
#include "Date.h"
#include "Event.h"
#include "Calendar.h"
#include <algorithm>

using namespace std;

/*
bool compare(Event& lhs, Event& rhs) {
    if (lhs.startYear < rhs.startYear)
        return true;
    if (lhs.startYear == rhs.startYear && lhs.startMonth < rhs.startMonth)
        return true;
    if (lhs.startYear == rhs.startYear && lhs.startMonth == rhs.startMonth && lhs.startDay < rhs.startDay)
        return true;
    if (lhs.startYear == rhs.startYear && lhs.startMonth == rhs.startMonth && lhs.startDay == rhs.startDay && lhs.eventName < rhs.eventName)
        return true;
    return false;
}*/

int main()
{
    Calendar cal;
    //cal.startMenu();
    //cal.addEvent();
    //cal.sortFile();
    //cal.readFromFileInsertToVector();
   //cal.listEvents();
   cal.deleteEvent();
   



    
}

