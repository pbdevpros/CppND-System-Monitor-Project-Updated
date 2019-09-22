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

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    float total_up_time = LinuxParser::UpTime(pid_) / HZ ;
    float active_time = LinuxParser::ActiveJiffies(pid_) / HZ ;
    time_spent_on_process;
    time_spent_on_process = 1; // TODO: find calculation
   
    // calculation
    float time_process_running =  ( system_up_time - process_start_time ) / HZ ;
    time_spent_on_process /= HZ ;
    return ( time_spent_on_process / time_process_running ) * 100; // get %
}

string Process::Command() { return LinuxParser::Command(pid_) }

string Process::Ram() { return std::string Ram(int pid); }

string Process::User() { return LinuxParser::User(pid_) }

long int Process::UpTime() { return LinuxParser::UpTime(pid_) }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }