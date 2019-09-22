#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  Processor() ;
  float Utilization();

 private:
  void SaveUtilizationStatistics();
  int total_ {0}; // total time running
  int idle_ {0}; // total idle time
};

#endif