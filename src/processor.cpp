#include "processor.h"
#include <unordered_map>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{ 
    int prevIdle = idle_;
    int prevTotal = total_;
    saveUtilizationStatistics();
    int diffTotal = total_ - prevTotal;
    int diffIdle = idle_ - prevIdle;
    return ( (diffTotal - diffIdle) / diffTotal ) ;
}

/// @throws 255 CPU stats file read incorrectly
void Processor::saveUtilizationStatistics()
{
    std::unordered_map<std::string, int> CPUstats = {
        { "tUser" , 0 },
        { "tNice", 0 },
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
        if (element.second < 0 ) {
            throw 255;
        }
        linestream >> element.second;
    }

    // get idle and non-idle time
    idle_ = CPUstats["tIdle"] + CPUstats["tIOwait"];
    total_= CPUstats["tUser"] + CPUstats["tNice"] + CPUstats["tSystem"] + CPUstats["tIrq"] + CPUstats["tSoftIrq"] + CPUstats["tSteal"] + idle_;
}