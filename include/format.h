#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
    std::string ElapsedTime(long times);  
    static constexpr int MINUTE = 60; // taking second as base 
    static constexpr int HOUR = 60 * MINUTE;
    static constexpr int DAY = 24 * HOUR * MINUTE;
};                                    // namespace Format

#endif