//
// Created by AwokenOwen on 4/13/26.
//

#pragma once
#include "Manager.h"
#include <fstream>

/**
 * @brief Singleton Macro
 */
#define Log LogManager::getInstance()

/**
 * @brief Singleton class that handles all logs
 */
class LogManager : public Manager{
    friend class GameManager;
public:
    /**
     * @brief Getter for the singleton instance
     *
     * @return The singleton instance of the GameManager
     */
    static LogManager& getInstance();

    /**
     * @brief Function to log a normal log into the log file
     *
     * @param fmt the message to be logged
     * @param ... any variables in the log
     */
    void log(const char* fmt, ...);
    /**
     * @brief Function to log a warning log into the log file
     *
     * @param fmt the message to be logged
     * @param ... any variables in the log
     */
    void logWarning(const char* fmt, ...);
    /**
    * @brief Function to log an error log into the log file
     *
     * @param fmt the message to be logged
     * @param ... any variables in the log
     */
    void logError(const char* fmt, ...);
private:
    /**
     * @brief Starts all other managers preparing to start the game
     *
     * @return 0 if successful and 1 if unsuccessful
     */
    int initialize() override;
    /**
     * @brief Shuts down all other managers and frees necessary data
     */
    void terminate() override;
    /**
     * @brief Private default constructor for singleton functionality
     */
    LogManager() = default;
    /**
     * @brief Private default deconstructor for singleton functionality
     */
    ~LogManager() override = default;

    /**
     * @brief The file the manager writes to
     */
    std::ofstream m_logFile{};
};

