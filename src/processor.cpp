#include "processor.h"


// Return the aggregate CPU utilization
// According to the formula (delta(total)-delta(idle))/(delta(total))
float Processor::Utilization() 
{
    LinuxParser::Jiffies(Total);
    float Percent = ((Total[1]-PrevTotal[1])-(Total[0]-PrevTotal[0]))*1.0F/(Total[1]-PrevTotal[1]);
    PrevTotal[0]=Total[0];
    PrevTotal[1]=Total[1];
    return 1-Percent;
}
