//
// Created by AwokenOwen on 4/13/26.
//

#include "LogManager.h"
#include <stdarg.h>

int LogManager::initialize() {
    // Open the file
    m_logFile.open("log.txt", std::ios::out);
    // Make sure the file is open before saying it's initialized
    if (!m_logFile.is_open()) {
        //Log failure to open file
        logError("Failed to open log file");
        return 1;
    }
    // Log done
    log("LogManager Initialized");
    return 0;
}

void LogManager::terminate() {
    // Log done before actually closing file so it actually gets written
    log("LogManager Terminated");
    m_logFile.close();
}

LogManager & LogManager::getInstance() {
    // Get singleton and return it
    static LogManager singleton;
    return singleton;
}

void LogManager::log(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // Format the string into a buffer
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    va_end(args);

    // Start the log with "Log: " to show it's a log
    m_logFile << "Log: " << buffer << std::endl;
}

void LogManager::logWarning(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // Format the string into a buffer
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    va_end(args);

    // Start the log with "Log: " to show it's a log
    m_logFile << "Warning: " << buffer << std::endl;
}

void LogManager::logError(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // Format the string into a buffer
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    va_end(args);

    // Start the log with "Log: " to show it's a log
    m_logFile << "Error: " << buffer << std::endl;
}
