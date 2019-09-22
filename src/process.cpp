#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"

using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

float Process::CpuUtilization() const {
    float prev_total_up_time = LinuxParser::UpTime(pid_) / HZ;
    float prev_active_time = LinuxParser::ActiveJiffies(pid_) / HZ ;
    sleep(1); // delay
    float total_up_time = LinuxParser::UpTime(pid_) / HZ ;
    float active_time = LinuxParser::ActiveJiffies(pid_) / HZ ;
    total_up_time -= prev_total_up_time;
    active_time -= prev_active_time;
    return ( 100 * active_time / total_up_time );
}

std::string Process::Command() { return LinuxParser::Command(pid_); }

std::string Process::Ram() { return LinuxParser::Ram(pid_); }

std::string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const { 
    if ( CpuUtilization() < a.CpuUtilization() ) return true; 
    return false;
}