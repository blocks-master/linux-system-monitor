
#include <string>

#include "format.h"

using std::string;
using std::to_string;


// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) 
{
    int hour = seconds / 3600;
    int min = (seconds % 3600) / 60;
    int sec = (seconds %3600) % 60;
    
    string hour_str = string(2 - to_string(hour).length(), '0') + to_string(hour);
    string min_str = string(2 - to_string(min).length(), '0') + to_string(min);
    string sec_str = string(2 - to_string(sec).length(), '0') + to_string(sec);
    return hour_str + ":" + min_str + ":" + sec_str;
}