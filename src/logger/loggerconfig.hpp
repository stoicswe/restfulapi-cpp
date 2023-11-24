//
//  loggerconfig.hpp
//  simplelogger
//
//  Created by Nathaniel Knudsen on 11/20/23.
//
#include <string>

#include "loggertypes.hpp"

#ifndef loggerconfig_h
#define loggerconfig_h

namespace simplelogger {

    class LoggerConfig {
    public:
        LoggerConfig() : m_maxLogLevel(INFO) {
            
        }
        LoggerConfig(LogLevel lvl) {
            m_maxLogLevel = lvl;
        }
        
        void setMaxLogLevel(LogLevel lvl);
        
        void setDateTimeFormat(std::string& dtformat);
        
        std::string getDateTimeFormat();
        
        /**
         * Returns the currently maximum log level that is set for this current logger. Logging messages that are
         * not less than the maximum log level will not be shown to the console.
         *
         * @return LogLevel
         */
        LogLevel getMaxLogLevel();
    private:
        LogLevel m_maxLogLevel;
        std::string m_dtformat;
    };

}

#endif /* loggerconfig_h */
