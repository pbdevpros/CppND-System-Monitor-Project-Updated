#include <unistd.h>
#include <cstddef>
#include <set>

#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  cpu_ = Processor();
  Processes(); // load processses
  os_ = LinuxParser::OperatingSystem();
  kernel_ = LinuxParser::Kernel();
}

Processor& System::Cpu() { 
    return cpu_; 
}

vector<Process>& System::Processes() {
    if (!processes_.empty()) {
        processes_.clear();
    }
    vector<int> pids = LinuxParser::Pids();
    for (auto pid : pids ) {
        processes_.emplace_back(Process(pid));
    }
  	std::sort(processes_.begin(), processes_.end(), std::greater<Process>() );
    return processes_;
}

std::string System::Kernel() { 
    return kernel_;
}

float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization();
}

std::string System::OperatingSystem() { 
    return os_; 
}

int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();
}

int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

long int System::UpTime() { 
    return LinuxParser::UpTime();
}