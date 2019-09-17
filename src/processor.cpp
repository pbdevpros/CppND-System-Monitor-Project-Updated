#include "processor.h"
#include <unordered_map>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{ 
    std::unordered_map<std::string, int> CPUstats = {
        { "User" , 0 },
        { "Nice", 0 },
        { "tSystem", 0},
        { "tIdle", 0},
        { "tIOwait", 0},
        { "tIrq", 0 },
        { "tSoftIrq" , 0},
        { "tSteal", 0},
        { "tGuest", 0},
        { "tGuestNice", 0}
    };

    // parse /proc/stat file for information on CPU utilization
    std::string key {"cpu"}, filepath {LinuxParser::kProcDirectory + LinuxParser::kStatFilename};
    auto value = LinuxParser::ParseFileForLineWithKey(filepath, key);

    std::istringstream linestream(value);
    for ( auto element : CPUstats ) {
        linestream >> element.second;
    }

    // TODO: use algo to calculate the actual stats and save as member variables....
}