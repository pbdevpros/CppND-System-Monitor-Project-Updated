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
    float new_total = LinuxParser::UpTime(pid_) / HZ ;
    float new_active = LinuxParser::ActiveJiffies(pid_) / HZ ;
    new_total -= total_ / HZ;
    new_active -= active_ / HZ ;
    return ( new_active / new_total );
}

std::string Process::Command() { return LinuxParser::Command(pid_); }

std::string Process::Ram() { return LinuxParser::Ram(pid_); }

std::string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const { 
    return CpuUtilization() < a.CpuUtilization();
}