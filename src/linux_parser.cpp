#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template<typename T>
T ParseFileForKey(std::string filepath, std::string key);

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() 
{ 
  float totalUsedMem;
  int memTotal, memFree;
  std::vector<std::string> kMemTotal {"MemTotal" }, kMemFree {"MemFree"};
  
  // parse files for key
  memTotal = ParseFileForKey<int>(kProcDirectory + kMeminfoFilename, kMemTotal);
  memFree = ParseFileForKey<int>(kProcDirectory + kMeminfoFilename, kMemFree);

  // calclate % mem utilized
  if (memTotal && memFree) {
    totalUsedMem = (memTotal - memFree) / memTotal;
    return totalUsedMem; 
  } else {
    throw 255; // error parsing file
  }
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

int LinuxParser::TotalProcesses() 
{ 
  int totalProcesses;
  std::string kTotalProcesses {"processes"};
  totalProcesses = ParseFileForKey(kProcDirectory + kStatFilename, kTotalProcesses );

  if (totalProcesses) {
    return totalProcesses;
  } else {
    throw 255;
  }
}

int LinuxParser::RunningProcesses() 
{ 
  int runningProcesses;
  std::string kRunningProcesses{"procs_running"};
  runningProcesses = ParseFileForKey(kProcDirectory + kStatFilename, kRunningProcesses );
  if (runningProcesses) {
    return runningProcesses;
  } else {
    throw 255;
  }
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }


// ===========================================================================================================================
//                                                          UTILITY FUNCTIONS
// ===========================================================================================================================

template<typename T>
T ParseFileForKey(std::string filepath, std::string key)
{
  std::string param;
  T value;
  
  // parse file by each line
  std::ifstream filestream(filepath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> param >> value) {
        if (param == key) {
          return value;
        }
      }
    }
  }

  // could not find key
  return NULL;
}

std::string ParseFileForLineWithKey(std::string filepath, std::string key)
{
  std::string lineWithKey, param, value, line;
  
  // parse file by each line
  std::ifstream filestream(filepath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> param >> value) {
        if (param == key) {
          lineWithKey = line;
        }
      }
    }
  }

  return lineWithKey;
}