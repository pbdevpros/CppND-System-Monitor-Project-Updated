#include "unit_test.h"

int UnitTest::myLinuxParserTest() {
    std::cout << "Running LinuxParserTest\nReading CPU Data..." << std::endl;
    auto active_jiffies = LinuxParser::ReadCPUstats(0);
    std::cout << "Total cpu usage is: \t" << Format::ElapsedTime(active_jiffies) << std::endl ;
    std::cout << "Testing the MemInfo" << std::endl;
    float mem = LinuxParser::MemoryUtilization() ;
    std::cout << "Memory Info : "  << mem << std::endl;
    std::cout << "Test the UpTime" << std::endl;
    long uptime = LinuxParser::UpTime();
    std::cout << "Uptime is: " << uptime << std::endl; 
    return 0;
}