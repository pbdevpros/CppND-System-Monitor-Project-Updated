#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unordered_map>
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

long LinuxParser::UpTime() { 
  std::string line, token;
  std::ifstream f(kProcDirectory + kUptimeFilename) ;
  if (f.is_open()) {
      std::getline(f, line);
      std::istringstream linestream(line);
      linestream >> token;
  }
  return (long) stof(token); 
}

long LinuxParser::Jiffies() { 
  return ReadCPUstats(2) 
}

long LinuxParser::ActiveJiffies() {
  return ReadCPUstats(0); 
}

long LinuxParser::IdleJiffies() {
  return ReadCPUstats(1); 
}

vector<string> LinuxParser::CpuUtilization() { 
  // get the change in utilization of a given period of time
  auto prev_total = Jiffies();
  auto prev_idle = IdleJiffies();
  sleep(WAITTIME); // delay
  
  auto total = Jiffies();
  auto idle = IdleJiffies();
  
  total -= prev_total;
  idle -= prev_idle;

  // calculate  utilization
  auto utilization = ( total - idle ) / total;
  return Format::ElapsedTime(utilization);
}

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

string LinuxParser::Command(int pid) {
  string pid_string = to_string(pid); 
  string command ;
  std::fstream stream ( kProcDirectory + pid_string + kCmdlineFilename );
  if ( stream.is_open()) {
    getline(stream, command);
    return command;
  }
}

string LinuxParser::Ram(int pid) {
  string key = "VmSize:"
  auto memKB = stol(ParseFileForKey( kProcDirectory + to_string(pid) kStatusFilename, key));
  float memMB = memKB / 1024 ; 
  return to_string(memMB);
}

string LinuxParser::Uid(int pid) { 
  string key = "Uid:"
  auto Uid = ParseFileForKey( kProcDirectory + to_string(pid) + kStatusFilename, key);
  return Uid;
}

string LinuxParser::User(int pid[[maybe_unused]]) { 
  string line ;
  string username {} ;
  auto Uid = Uid(pid);
  std::ifstream fstream ( kPasswordPath );
  if (fstream.is_open()) {
    while (getline( fstream, line ) ) {
      auto found = line.find(Uid);
      if ( found != std::string::npos ) {
        username = line.substr(0, line.find(":"));
        return username;
      }
    }
  }
  return username;
}

long LinuxParser::UpTime(int pid) { 
  long int sys_uptime = UpTime();
  long int pid_uptime ;
  string line ;
  int counter, field = 22 ; // pid up time is the 22nd field in /proc/[pid]/stat
  
  // read the time when the pid began
  std::ifstream fstream ( kProcDirectory + to_string(pid) + kStatFilename )
  if ( fstream.is_open() ) {
    getline( fstream, line ) ;
    int index = line.find(" ");
    while ( index != std::string::npos ) {
      if (counter == (field - 1) ) { 
        // difference between time when system and process went up
        pid_uptime = stol(line.substr(0, index)) ; 
        return (sys_uptime - pid_uptime) ; 
      }
      line = line.substr(index+1);
      index = line.find(" ") ;
      counter++;
    } 
  }

  return 0;
}

long LinuxParser::ActiveJiffies(int pid) { 
  int counter ;
  enum statIndex { // defines the field of /proc/[pid]/stat associated with value
    kUtime = 14,
    kStime,
    kCUtime,
    kCStime
  };
  long utime, stime, cutime, cstime ;

  // read the time when the pid began
  std::ifstream fstream ( kProcDirectory + to_string(pid) + kStatFilename )
  if ( fstream.is_open() ) {
    getline( fstream, line ) ;
    int index = line.find(" ");
    while ( index != std::string::npos ) {
      if (counter == kUtime - 1 ) utime = stol(line.substr(0, index))
      if (counter == kStime - 1 ) stime = stol(line.substr(0, index))
      if (counter == kCUtime - 1 ) cutime = stol(line.substr(0, index))
      if (counter == kCStime - 1 ) { cstime = stol(line.substr(0, index)); break }
      line = line.substr(index+1);
      index = line.find(" ") ;
      counter++;
    } 
  }
  utime += stime + cutime + cstime;
  return utime;
}

// ===========================================================================================================================
//                                                          UTILITY FUNCTIONS
// ===========================================================================================================================

template<typename T>
T ParseFileForKey(std::string filepath, std::string key)
{
  std::string param, line;
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

/// @brief Load total CPU stats from /proc/stat and return number of jiffies
/// @param jiffyType Integer, can be one of:
///                            0 - returns active jiffies
///                            1 - returns idle jiffies
///                            2 - returns total jiffies
/// @throws 255, if parameter jiffyType is not one of the possible integers                         
long LinuxParser::ReadCPUstats(int jiffyType)
{
  std::unordered_map<CPUStates, long> CPUstats = {
    { kUser_ , 0 },
    { kNice_ , 0 },
    { kSystem_, 0},
    { kIdle_, 0},
    { kIOwait_, 0},
    { kIRQ_, 0 },
    { kSoftIRQ_ , 0},
    { kSteal_, 0},
    { kGuest_, 0},
    { kGuestNice_, 0}
  };

  // parse /proc/stat file for information on CPU utilization
  std::string key {"cpu"}, filepath {LinuxParser::kProcDirectory + LinuxParser::kStatFilename};
  auto value = ParseFileForLineWithKey(filepath, key);

  std::istringstream linestream(value);
  for ( auto element : CPUstats ) {
      if (element.second < 0 ) {
          throw 255;
      }
      linestream >> std::stol(element.second);
  }

  if ( jiffyType == 0 ) { // return active jiffies
    return ( stats[kUser_] + stats[kNice_] + stats[kSystem_] + stats[kIRQ_] + stats[kSoftIRQ_] + stats[kSteal_] ) ;
  } else ( jiffyType == 1 ) { // return idle jiffies
    return ( stats[kIdle_] + stats[kIOwait_] );
  } else ( jiffyType == 2 ) { // return total jiffies
    return ( stats[kUser_] + stats[kNice_] + stats[kSystem_] + stats[kIRQ_] + stats[kSoftIRQ_] + stats[kSteal_] + stats[kIdle_] + stats[kIOwait_] ) ;
  } else {
    throw 255;
  }
}