//
//  SimpleLogger.cpp
//  restfulapicpp
//
//  Created by Nathaniel Knudsen on 11/20/23.
//

#include "simplelogger.hpp"

using namespace simplelogger;

void ConsoleLogger::append(LogRecord record) {
    // TODO: Get a lock
    records->push_back(record);
    // TODO: Free a lock
}

void ConsoleLogger::flush() {
    // TODO: Get a lock
    for(LogRecord r : *records) {
        // TODO: Flesh out the flush func
        // this will be called when the logger is destroyed or
        // if called sooner for some reason.
    }
    // TODO: Free lock
}
