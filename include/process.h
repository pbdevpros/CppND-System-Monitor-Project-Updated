#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unistd.h>
#include "linux_parser.h"
#include "logger.h"

class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User(); 
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  bool operator>(const Process& a) const;
  float HZ {};

 private:
  int pid_ ;
  float uptime_ {0}; // total time running
  float active_ {0}; // total active time
};

#endif