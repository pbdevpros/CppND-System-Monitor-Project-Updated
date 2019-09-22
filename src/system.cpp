#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { 
    return cpu_; 
}

vector<Process>& System::Processes() {
    if (!processes_.empty()) {
        processes_.clear();
    }
    vector<int> pids = LinuxParser::Pids();
    for (auto pid : pids ) {
        processes_.push_back(Process(pid));
    }
    return processes_  
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