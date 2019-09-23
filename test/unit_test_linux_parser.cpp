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

int UnitTest::PIDUsernameTest()
{
    std::cout << "Testing the User(pid) function." << std::endl;
    int pid = 1;
    std::string username = LinuxParser::User(pid);
    std::cout << "username is: " << username << std::endl;
    return 0;
}

int UnitTest::TotalProcessesTest()
{
    std::cout << "Testing the Total Processes function." << std::endl;
    int num = LinuxParser::TotalProcesses() ;
    std::cout << "Total Processes is: " << num << std::endl;
    return 0;
}

int UnitTest::RunningProcessesTest()
{
    std::cout << "Testing the LinuxParser::RunningProcesses() function." << std::endl;
    int num = LinuxParser::RunningProcesses() ;
    std::cout << "Running Processes is: " << num << std::endl;
    return 0;
}