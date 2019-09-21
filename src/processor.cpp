#include "processor.h"

float Processor::Utilization() 
{ 
    int prevIdle = idle_;
    int prevTotal = total_;
    SaveUtilizationStatistics();
    int diffTotal = total_ - prevTotal;
    int diffIdle = idle_ - prevIdle;
    return ( (diffTotal - diffIdle) / diffTotal ) ;
}

/// @throws 255 CPU stats file read incorrectly
void Processor::SaveUtilizationStatistics()
{
    idle_ = LinuxParser::IdleJiffies();
    total_ = LinuxParser::Jiffies();
}