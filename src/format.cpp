#include "format.h"

static constexpr int MINUTE = 60; // taking second as base 
static constexpr int HOUR = 60 * MINUTE;
static constexpr int DAY = 24 * HOUR * MINUTE;

std::string Format::ElapsedTime(long seconds) { 
    std::string ftime;
    if (seconds > ( DAY- 1) ) {
        ftime = "23:59:59"; // max time reached!
    } else {
        long fseconds = seconds % MINUTE;
        long fminutes = ( (long) seconds / MINUTE ) % MINUTE;
        long fhours = (long) seconds / HOUR;
        ftime = std::to_string(fhours) + ":" + std::to_string(fminutes) + ":" + std::to_string(fseconds) ;
    }
    return ftime; 
}