#include <string>

#include "format.h"

using std::string;

// helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) 
{
    string uptime;
    int hour;
    int minute;
    int sec;
    hour=seconds/3600;
    minute=(seconds-hour*3600)/60;
    sec=seconds-hour*3600-minute*60;
    uptime=SetZero(hour)+": "+SetZero(minute)+": "+SetZero(sec);
    return uptime;

}

string Format::SetZero(int time)
{
    if(time<10)
    {
        return "0"+std::to_string(time);
    }
    return std::to_string(time);
}