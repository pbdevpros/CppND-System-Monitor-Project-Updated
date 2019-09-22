#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unistd.h>

class Process {
 public:
  Process(int);
  int Pid();
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  float HZ {};

 private:
    int pid_ ;
};

Process::Process(int pid) {
    HZ = sysconf(_SC_CLK_TCK);
    pid_ = pid;
}

#endif