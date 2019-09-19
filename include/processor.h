#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  Processor() ;
  float Utilization();  // TODO: See src/processor.cpp

 private:
  void saveUtilizationStatistics();
  int total_ {0}; // total time running
  int idle_ {0}; // total idle time
};

Processor::Processor() 
{
  try {
    saveUtilizationStatistics();
  } catch (int e) {
    throw;
  }
}

#endif