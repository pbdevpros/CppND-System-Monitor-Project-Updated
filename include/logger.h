#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <string>
#include <fstream>

using std::string;

#define myLogger(level, message) {\
    string lev = level ? "DEBUG" : "ERROR";\
    std::ofstream ofs;\
    ofs.open("/tmp/logfile.txt", std::ofstream::out | std::ofstream::app);\
    ofs << "FILE: " << __FILE__ << " | LINE " << __LINE__ << " | <" << lev << "> " << message << "\n";\
    ofs.close();\
}

#endif