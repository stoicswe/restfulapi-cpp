//
//  SimpleLogger.hpp
//  restfulapicpp
//
//  Created by Nathaniel Knudsen on 11/20/23.
//
#include <stdio.h>
#include <string>
#include <mutex>

#include "loggerconfig.hpp"

#ifndef SimpleLogger_hpp
#define SimpleLogger_hpp

namespace simplelogger {

    class LoggerAggregator {
        
        friend class Logger;
        
    public:
        LoggerAggregator();
        virtual ~LoggerAggregator();
        
    protected:
        virtual void append(LogRecord record);
        virtual void flush();
        std::shared_ptr<std::vector<LogRecord>> records;
    };

    class Logger {
        // TODO: Investigate the use of a thread pool to create non-blocking calls
        // This may be needed for adding records to the record vector vs flushing the
        // record vector.
    public:
        Logger(LoggerConfig config) : m_config(config) {
            aggregators = std::make_shared<std::vector<LoggerAggregator*>>();
        }
        ~Logger() {
            if(aggregators) {
                for(LoggerAggregator* logger : *aggregators) {
                    logger->flush();
                }
            }
        }
        void logLvl(LogLevel& lvl) {
            clvl = lvl;
        }
        void operator<<(std::string cont) {
            for(LoggerAggregator* logger : *aggregators) {
                logger->append(LogRecord(clvl, cont));
            }
        }
        void appendAggregator(LoggerAggregator* aggregator) {
            aggregators->push_back(aggregator);
        }
        LoggerConfig getLogConfig() {
            return this->m_config;
        }
    private:
        std::shared_ptr<std::vector<LoggerAggregator*>> aggregators;
        LoggerConfig m_config;
        volatile LogLevel clvl;
    };

    // SimpleLogger instance for the namespace when using the
    // simple logger within a project.
    static std::shared_ptr<Logger> loggerInstance = nullptr;

    class ConsoleLogger : public LoggerAggregator {
    public:
        ConsoleLogger() : LoggerAggregator() {
            records = std::make_shared<std::vector<LogRecord>>();
            m_config = loggerInstance->getLogConfig();
        }
        ~ConsoleLogger() {
        }
        void append(LogRecord record) override;
        void flush() override;
    private:
        LoggerConfig m_config;
    };

    static Logger LOG(LogLevel lvl) {
        loggerInstance->logLvl(lvl);
        return *loggerInstance;
    }

    static void LOG_INIT(LoggerConfig config) {
        if(loggerInstance == nullptr) {
            loggerInstance = std::make_shared<Logger>(config);
        }
    }

    static void LOG_ADD_AGGREGATOR(LoggerAggregator* aggregator) {
        loggerInstance->appendAggregator(aggregator);
    }

}


#endif /* SimpleLogger_hpp */
