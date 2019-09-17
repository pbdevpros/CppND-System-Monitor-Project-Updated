#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  Processor() ;
  float Utilization();  // TODO: See src/processor.cpp

 private:
  int prevTotal {0}; // total time running
  int prevIdle {0}; // total idle time
};

Processor::Processor() 
{
    // TODO: Assign member variables...
}

#endif