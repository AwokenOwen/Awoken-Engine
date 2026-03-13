/*
 * Written by: AwokenOwen
 * Last Updated: March 13th 2026
 */

#pragma once

/**
 * @brief Singleton Macro
 */
#define Game GameManager::getInstance()

/**
 * @brief Singleton class that runs the engine
 */
class GameManager
{
public:
	/**
	 * @brief Start all other Managers and get them ready to run the game
	 *
	 * Starts up all managers and returns 0 if successful and 1 if failed
	 *
	 * @return int
	 */
	static int initialize();

	/**
	 * @brief Run the game loop
	 *
	 * Starts the while loop that keeps the window open and sends updates to all Objects
	 */
	void run();

	/**
	 * @brief Shut Down all Managers freeing all data
	 */
	static void terminate();

	//
	/**
	 * @brief GameManager Singleton get function
	 *
	 * @return static GameManager Instance
	 */
	static GameManager& getInstance();

	/**
	 * @brief Returns the time in seconds that the program has run
	 *
	 * @return float
	 */
	static float getTime();
	/**
	 * @brief Returns the calulated delta time of the previous frame
	 *
	 * @return float
	 */
	[[nodiscard]] float getDeltaTime() const;

	/**
	 * @brief Sets the while loop in run() to finish
	 */
	static void quit();

private:
	/**
	 * @brief Private Constructor for singleton functionality
	 */
	GameManager();

	/**
	 * @brief helper variable used to calculate m_deltaTime
	 *
	 * The result of getTime at the end of the last frame is used to find the difference between that and the time of the current frame to calculate m_deltaTime
	 */
	float m_timeOnLastFrame = 0;
	/**
	 * @brief The change in time updated between frames
	 *
	 * The difference between getTime() last frame and the current frame
	 */
	float m_deltaTime = 0;
};