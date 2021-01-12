#include <string>
#include <iomanip>
#include "format.h"

using std::string;

// helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) 
{
    int hour;
    int minute;
    int sec;
    hour=seconds/3600;
    minute=(seconds-hour*3600)/60;
    sec=seconds-hour*3600-minute*60;
    std::ostringstream elapsedTime;
    elapsedTime<<std::setw(2)<<std::setfill('0')<<hour
               <<":"<<std::setw(2)<<std::setfill('0')<<minute
               <<":"<<std::setw(2)<<std::setfill('0')<<sec;
    return elapsedTime.str();

}

