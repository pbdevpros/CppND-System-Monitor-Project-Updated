#include <dirent.h>
#include <unistd.h>
#include <vector>
#include <map>
#include "linux_parser.h"
#include <iostream>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

static constexpr int WAITTIME (1);
string ParseFileForKey(std::string filepath, std::string key);
string ParserFileForLineWithKey(string filename, string key);
string StrRemoveWhiteSpace(string target);
string StrRemoveKey(string target, string key, bool isAfter);
string StrReplaceString(string target, string sub);

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
  float totalUsedMem, memTotal, memFree;
  string key_mem_total ("MemTotal" ), key_mem_free ("MemFree");
  
  // parse /proc/meminfo for the given keys
  std::cout << "Memory Utilization function..." << std::endl ;
  string sMemTotal = ParserFileForLineWithKey(kProcDirectory + kMeminfoFilename, key_mem_total);
  string sMemFree = ParserFileForLineWithKey(kProcDirectory + kMeminfoFilename, key_mem_free);

  // parse through string for details
  bool isAfter = true;
  sMemTotal = StrRemoveKey(sMemTotal, key_mem_total, isAfter);
  sMemFree = StrRemoveKey(sMemFree, key_mem_free, isAfter);
  
  string misc_chars ("KB");
  sMemTotal = StrRemoveKey(sMemTotal, misc_chars, !isAfter);
  sMemFree = StrRemoveKey(sMemFree, misc_chars, !isAfter);

  sMemTotal = StrRemoveWhiteSpace(sMemTotal);
  sMemFree = StrRemoveWhiteSpace(sMemFree);

  memTotal = std::stoi(sMemTotal);
  memFree = std::stoi(sMemFree);

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
  return ReadCPUstats(2);
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
  vector<string> cpu_utils ;
  cpu_utils.push_back(Format::ElapsedTime(utilization));
  return cpu_utils;
}

int LinuxParser::TotalProcesses() 
{ 
  int totalProcesses;
  std::string kTotalProcesses ("processes");
  totalProcesses = std::stoi(ParseFileForKey(kProcDirectory + kStatFilename, kTotalProcesses ));

  if (totalProcesses) {
    return totalProcesses;
  } else {
    throw 255;
  }
}

int LinuxParser::RunningProcesses() 
{ 
  int runningProcesses;
  std::string key ("procs_running");
  runningProcesses = std::stoi(ParseFileForKey(kProcDirectory + kStatFilename, key));
  if (runningProcesses) {
    return runningProcesses;
  } else {
    throw 255;
  }
}

string LinuxParser::Command(int pid) {
  string pid_string (to_string(pid)); 
  string command ;
  std::fstream stream ( kProcDirectory + pid_string + kCmdlineFilename );
  if ( stream.is_open()) {
    getline(stream, command);
  }
  return command;
}

string LinuxParser::Ram(int pid) {
  string key ("VmSize:");
  float memKB = std::stof(ParseFileForKey( kProcDirectory + to_string(pid) + kStatusFilename, key));
  float memMB = memKB / 1024 ; 
  return to_string(memMB);
}

string LinuxParser::Uid(int pid) { 
  string key ("Uid:");
  auto uid = ParseFileForKey( kProcDirectory + to_string(pid) + kStatusFilename, key);
  return uid;
}

string LinuxParser::User(int pid[[maybe_unused]]) { 
  string line ;
  string username {} ;
  string uid = Uid(pid);
  std::ifstream fstream ( kPasswordPath );
  if (fstream.is_open()) {
    while (getline( fstream, line ) ) {
      auto found = line.find(uid);
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
  int counter = 0;
  int field = 22 ; // pid up time is the 22nd field in /proc/[pid]/stat
  
  // read the time when the pid began
  std::ifstream fstream ( kProcDirectory + to_string(pid) + kStatFilename );
  if ( fstream.is_open() ) {
    getline( fstream, line ) ;
    size_t index = line.find(" ");
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
  int counter = 0;
  enum statIndex { // defines the field of /proc/[pid]/stat associated with value
    kUtime = 14,
    kStime,
    kCUtime,
    kCStime
  };
  long utime = 0;
  long stime = 0,  cutime = 0, cstime = 0;
  std::string line;

  // read the time when the pid began
  std::ifstream fstream ( kProcDirectory + to_string(pid) + kStatFilename );
  if ( fstream.is_open() ) {
    getline( fstream, line ) ;
    size_t index = line.find(" ");
    while ( index != std::string::npos ) {
      if (counter == kUtime - 1 ) utime = std::stol(line.substr(0, index)) ;
      if (counter == kStime - 1 ) stime = std::stol(line.substr(0, index)) ;
      if (counter == kCUtime - 1 ) cutime = std::stol(line.substr(0, index)) ;
      if (counter == kCStime - 1 ) { cstime = std::stol(line.substr(0, index)); break; }
      line = line.substr(index+1);
      index = line.find(" ") ;
      counter++;
    } 
    utime += stime + cutime + cstime;
  }
  return utime;
}

// ===========================================================================================================================
//                                                          UTILITY FUNCTIONS
// ===========================================================================================================================

string ParseFileForKey(std::string filepath, std::string key)
{
  std::cout << "Inside the ParseFileForKey function," ;
  std::string param, line, value;
  // parse file by each line
  std::ifstream filestream(filepath);
  std::cout << "Deeper" ;
  if (filestream.is_open()) {
    std::cout << " filestream is open, " ;
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      auto found = line.find(key);
      std::cout << "\nThis is the line: " << line << std::endl;
      std::cout << "Found is:\t\t" << found << std::endl;
      if ( found != std::string::npos ) {
        value = line.substr( found + key.length() + 1, line.find(" ")); // find all characters after the key (and whitespace directly after it) and before the next whitespace.
        std::cout << " found " << key << " saving the value: " << value << std::endl;
        return value;
      }
    }
  }
  // could not find key
  std::cout << "\n\tCould not find the keyword!\t Exiting..." << std::endl;
  return NULL;
}


string ParserFileForLineWithKey(string filename, string key)
{
  std::string param, line, value;
  
  // parse file by each line
  std::ifstream filestream(filename);  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      auto found = line.find(key);
      std::cout << "This is the line: " << line << std::endl;
      if ( found != std::string::npos ) {
        return line;
      }
    }
  }

  return string();
}

/// @brief Load total CPU stats from /proc/stat and return number of jiffies
/// @param jiffyType Integer, can be one of:
///                            0 - returns active jiffies
///                            1 - returns idle jiffies
///                            2 - returns total jiffies
/// @throws 255, if parameter jiffyType is not one of the possible integers                         
long LinuxParser::ReadCPUstats(int jiffyType)
{
  std::map<CPUStates, long> stats = {
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
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string line ; 
  if (filestream.is_open()) {
    getline(filestream, line); // first line contains info about overall CPU usage...
  }
  std::cout << "Printing the first line of /proc/stat/ : " << line << std::endl;

  string param;
  std::istringstream linestream(line);
  linestream >> param; // first token is the word cpu
  std::map<CPUStates, long>::iterator itr;

  for ( itr =  stats.begin(); itr != stats.end(); ++itr ) {
      linestream >> param;
      itr->second = std::stol(param);
      std::cout << "This is stat[ " << itr->first << "] = " << itr->second << ",\t is itr->second a long? \t" << sizeof(long)/sizeof(itr->second) << std::endl;
      if (itr->second < 0 ) {
          throw 255;
      }
  }

  if ( jiffyType == 0 ) { // return active jiffies
    std::cout << "Active Jiffies being calculated..." << std::endl;
    return ( stats[kUser_] + stats[kNice_] + stats[kSystem_] + stats[kIRQ_] + stats[kSoftIRQ_] + stats[kSteal_] ) ;
  } else if ( jiffyType == 1 ) { // return idle jiffies
    std::cout << "Idle Jiffies being calculated..." << std::endl;
    return ( stats[kIdle_] + stats[kIOwait_] );
  } else if ( jiffyType == 2 ) { // return total jiffies
    std::cout << "Total Jiffies being calculated..." << std::endl;
    return ( stats[kUser_] + stats[kNice_] + stats[kSystem_] + stats[kIRQ_] + stats[kSoftIRQ_] + stats[kSteal_] + stats[kIdle_] + stats[kIOwait_] ) ;
  } else {
    throw 255;
  }
}

string StrRemoveWhiteSpace(string target)
{
  std::string::iterator end_pos = std::remove(target.begin(), target.end(), ' '); 
  target.erase(end_pos, target.end());
  return target;
}

/// @brief Removes a given key from a string.
/// @param isAfter When true, will return a substring of all characters after the key in the given string.
string StrRemoveKey(string target, string key, bool isAfter)
{
  if (isAfter){
    target = target.substr(target.find(key) + key.length() + 1); 
  } else {
    target = target.substr(0, target.find(key) - 1); 
  }

  return target;
}

/// @brief Replaces a given substring with whitespace
string StrReplaceString(string target, string substring)
{
  int len = substring.length();
  for ( int i = 0; i < len; i++ )
  {
    std::replace(target.begin(), target.end(), substring[i], ' ');
  }
  return target;
}