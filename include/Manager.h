//
// Created by AwokenOwen on 4/13/26.
//

#pragma once

/**
 * @brief Abstract base Class for all Managers
 */
class Manager {
public:
    /**
     * @brief default constructor
     */
    Manager() = default;
    /**
     * @brief default deconstructor
     */
    virtual ~Manager() = default;

private:
    /**
     * @brief Virtual initialize function will be called on startup of the engine
     *
     * @return 1 if successful and 0 if unsuccessful
     */
    virtual int initialize() = 0;
    /**
     * @brief Virtual terminate function will be called on shutdown of the engine
     */
    virtual void terminate() = 0;
};


