#ifndef XVM_LOGGER_HPP
#define XVM_LOGGER_HPP

#include <string>

namespace xvm {
    void initLogger(bool logToFile = false, const std::string& logFile = "logs/xvm.log");
}

#endif