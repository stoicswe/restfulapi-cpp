//
//  loggertypes.hpp
//  simplelogger
//
//  Created by Nathaniel Knudsen on 11/20/23.
//
#include <ctime>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

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

std::string getLogLevelStr(LogLevel level) {
    std::string str;
    switch (level) {
        case ERROR:
            str = "ERROR";
            break;
        case WARNING:
            str = "WARNING";
            break;
        case DEBUG:
            str = "DEBUG";
            break;
        case INFO:
            str = "INFO";
            break;
        case TRACE:
            str = "TRACE";
            break;
        default:
            str = "UNKNOWN";
            break;
    }
    return str;
}

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
    std::string toString(std::string dformat) {
        std::ostringstream date;
        time_t tt = convertTime();
        std::put_time(std::localtime(&tt), &dformat);
        std::string log = "";
        log += date.str();
        log += " [" + getLogLevelStr(m_lvl) + "]: ";
        log += m_cont + "\n";
        return log;
    }
private:
    LogLevel m_lvl;
    std::string m_cont;
    std::chrono::time_point<std::chrono::system_clock> m_creation;
    
    double convertTime() {
        auto tp_sec = std::chrono::time_point_cast<std::chrono::seconds>(m_creation);
        auto tt = std::chrono::system_clock::to_time_t (tp_sec);
        return tt;
    }
};

} /* namespace simplelogger */

#endif /* loggertypes_h */
