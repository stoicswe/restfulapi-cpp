//
//  loggerconfig.cpp
//  restfulapicpp
//
//  Created by Nathaniel Knudsen on 11/20/23.
//

#include "loggerconfig.hpp"

using namespace simplelogger;

void LoggerConfig::setMaxLogLevel(LogLevel lvl) {
    this->m_maxLogLevel = lvl;
}

LogLevel LoggerConfig::getMaxLogLevel() {
    return this->m_maxLogLevel;
}

void LoggerConfig::setDateTimeFormat(std::string &dtformat) {
    this->m_dtformat = dtformat;
}

std::string LoggerConfig::getDateTimeFormat() {
    return m_dtformat;
}

