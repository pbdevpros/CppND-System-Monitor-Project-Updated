#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
    float prev_total_up_time = LinuxParser::UpTime(pid_) / HZ;
    float prev_active_time = LinuxParser::ActiveJiffies(pid_) / HZ ;
    mdelay(LinuxParser::WAITTIME); // ms delay
    float total_up_time = LinuxParser::UpTime(pid_) / HZ ;
    float active_time = LinuxParser::ActiveJiffies(pid_) / HZ ;
    total_up_time -= prev_total_up_time;
    active_time -= prev_active_time;
    return ( 100 * active_time / total_up_time );
}

string Process::Command() { return LinuxParser::Command(pid_) }

string Process::Ram() { return std::string Ram(int pid); }

string Process::User() { return LinuxParser::User(pid_) }

long int Process::UpTime() { return LinuxParser::UpTime(pid_) }

bool Process::operator<(Process const& a) const { 
    if ( CpuUtilization() < a.CpuUtilization() ) return true; 
    return false;
}