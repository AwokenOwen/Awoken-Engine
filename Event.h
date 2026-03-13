/*
 * Written by: AwokenOwen
 * Last Updated: March 13th 2026
 */

#pragma once
#include <functional>
#include <vector>

using namespace std;

/**
 * @brief Event class for all events needed
 *
 * @tparam R the types of information being passed by the event
 */
template<typename... R>
class Event
{
public:
	/**
	 * Event Constructor
	 */
	Event();

	/**
	 * @brief Add a function as a listeners to this event
	 *
	 * Turns all functions into lambda functions so they can be stored as listeners
	 *
	 * @tparam T Class of member function
	 * @param object Pointer to the Object
	 * @param func The function getting added as a listener
	 */
	template<typename T>
	void add(T* object, void(T::* func)(R...));

	/**
	 * @brief Call all the functions that are listeners to the event
	 *
	 * @param args The inputs determined by R required to call the Event
	 */
	void callEvent(R... args);

private:
	/**
	 * @brief Vector of lambda functions ready to be called on callEvent()
	 */
	vector<function<void(R...)>> m_functions;
};

template<typename ...R>
Event<R...>::Event() = default;

template<typename ...R>
template<typename T>
inline void Event<R...>::add(T* object, void(T::* func)(R...))
{
	function<void()> lambda = [object, func](R... args)
	{
		(object->*func)(args...);
	};

	m_functions.push_back(lambda);
}

template<typename ...R>
inline void Event<R...>::callEvent(R ...args)
{
	for (int i = 0; i < m_functions.size(); i++)
	{
		m_functions[i](args...);
	}
}