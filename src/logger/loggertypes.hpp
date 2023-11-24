//
//  loggertypes.hpp
//  simplelogger
//
//  Created by Nathaniel Knudsen on 11/20/23.
//
#include <string>
#include <chrono>
#include <ctime>

#ifndef loggertypes_h
#define loggertypes_h

namespace simplelogger {

enum LogLevel {
    ERROR,
    WARNING,
    DEBUG,
    INFO,
    TRACE
};

class LogRecord {
public:
    LogRecord(LogLevel lvl, std::string& cont) : m_lvl(lvl), m_cont(cont) {
        m_creation = std::chrono::system_clock::now();
    }
    LogLevel getLevel() {
        return m_lvl;
    }
    std::string getMessage() {
        return m_cont;
    }
    std::chrono::time_point<std::chrono::system_clock> getCreation() {
        return m_creation;
    }
    std::string toString(std::string& format) {
        
    }
private:
    LogLevel m_lvl;
    std::string m_cont;
    std::chrono::time_point<std::chrono::system_clock> m_creation;
};

} /* namespace simplelogger */

#endif /* loggertypes_h */
