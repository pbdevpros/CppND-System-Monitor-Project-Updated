#include "processor.h"

Processor::Processor() 
{
  try {
    SaveUtilizationStatistics();
  } catch (int e) {
    throw;
  }
}

float Processor::Utilization() 
{ 
    float prevIdle = idle_;
    float prevTotal = total_;
    SaveUtilizationStatistics();
    float diffTotal = total_ - prevTotal;
    float diffIdle = idle_ - prevIdle;
    return ( (diffTotal - diffIdle) / diffTotal ) ;
}

/// @throws 255 CPU stats file read incorrectly
void Processor::SaveUtilizationStatistics()
{
    idle_ = LinuxParser::IdleJiffies();
    total_ = LinuxParser::Jiffies();
}