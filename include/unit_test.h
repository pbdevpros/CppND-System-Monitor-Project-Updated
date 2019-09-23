#ifndef UNITTEST_H
#define UNITTEST_H

#include <iostream>
#include <string>
#include <vector>
#include "linux_parser.h"

namespace UnitTest {
    int myLinuxParserTest();
    int PIDUsernameTest();
    int TotalProcessesTest();
    int RunningProcessesTest();
    int CPUUtilizationTest();
    int PIDRamTest();
    int PIDUptime();
    int PIDCPUUtil();
};

#endif