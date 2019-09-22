#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();              
  int TotalProcesses();         
  int RunningProcesses();       
  std::string Kernel();         
  std::string OperatingSystem();

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::string os_ {};
  std::string kernel_ {};
};

System::System() {
  cpu_ = Processor();
  Processes(); // load processses
  os_ = LinuxParser::OperatingSystem();
  kernel_ = LinuxParser::Kernel();
}

#endif