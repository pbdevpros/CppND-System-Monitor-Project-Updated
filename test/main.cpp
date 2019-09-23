// test functionality
#include "format.cpp"
#include "linux_parser.cpp"
#include "unit_test.h"

void testLinuxParser();

int main() {
    testLinuxParser();
}

void testLinuxParser()
{
    UnitTest::myLinuxParserTest();
    UnitTest::PIDUsernameTest();
    UnitTest::TotalProcessesTest();
    UnitTest::RunningProcessesTest();
    UnitTest::CPUUtilizationTest();
}