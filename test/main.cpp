// test functionality
#include "format.cpp"
#include "linux_parser.cpp"
#include "unit_test.h"

void testLinuxParser();
void testPIDs();

int main() {
    testLinuxParser();
    testPIDs();
}

void testLinuxParser()
{
    // UnitTest::myLinuxParserTest();
    UnitTest::PIDUsernameTest();
    // UnitTest::TotalProcessesTest();
    // UnitTest::RunningProcessesTest();
    // UnitTest::CPUUtilizationTest();
    // UnitTest::PIDRamTest();
}

void testPIDs()
{
    UnitTest::PIDUptime();
    UnitTest::PIDCPUUtil();
}