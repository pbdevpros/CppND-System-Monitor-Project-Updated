#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unistd.h>
#include "linux_parser.h"

class Process {
 public:
  Process(int);
  int Pid();
  std::string User(); 
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  float HZ {};

 private:
    int pid_ ;
};

Process::Process(int pid) {
    HZ = sysconf(_SC_CLK_TCK);
    pid_ = pid;
}

#endif