#include <string>

#include "format.h"

using std::string;


// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds)
{ 
    int hour = seconds / 3600;
    int second = seconds % 3600;
    int minute = second / 60;
    second %= 60;

    string time = std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second);
    return time;
}