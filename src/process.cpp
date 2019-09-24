#include <cctype>
#include <sstream>
#include <vector>
#include "process.h"

using std::to_string;
using std::vector;

Process::Process(int pid) {
    HZ = sysconf(_SC_CLK_TCK);
    pid_ = pid;
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() const {
    float uptime_new = LinuxParser::UpTime(pid_) ;
    float active_new = LinuxParser::ActiveJiffies(pid_) / HZ ;
    uptime_new -= uptime_ ;
    active_new -= active_ ;
    return ( active_new / uptime_new );
}

std::string Process::Command() { 
  std::string command = LinuxParser::Command(pid_); 
  if (command.length() > 40) {
     return (command.substr(0, 40) + "..."); 
  }
  return command;
}

std::string Process::Ram() { return LinuxParser::Ram(pid_); }

std::string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const { 
    if ( CpuUtilization() <  a.CpuUtilization()) return true;
  	return false;
}

bool Process::operator>(const Process& a) const {
    if (CpuUtilization() > a.CpuUtilization()) return true;
  	return false;
}