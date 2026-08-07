//
// Created by AwokenOwen on 4/13/26.

#pragma once
#include <functional>
#include <map>
#include <ranges>
#include <cstdint>

#include "LogManager.h"

/**
 * @brief A macro for adding and remove from specific events. Adding this wrapper in the public space allows events to be private but publicly added and removed from
 *
 * @param EventName The name of the event variable
 * @param ... the types of information being passed by the event
 */
#define EVENT_ACCESSORS(EventName, ...)                                          \
	template<typename T>                                                         \
	void add##EventName(T* object, void(T::* func)(__VA_ARGS__)) {              \
		EventName.add(object, func);                                             \
	}                                                                            \
	void addRaw##EventName(std::function<void(__VA_ARGS__)> function) {         \
		EventName.addRaw(std::move(function));                                   \
	}                                                                            \
	template<typename T>                                                         \
	void remove##EventName(T* object, void(T::* func)(__VA_ARGS__)) {           \
		EventName.remove(object, func);                                          \
	}


/**
 * @brief Event class for all events needed
 *
 * @tparam R the types of information being passed by the event
 */
template<typename... R>
class Event
{
public:
	Event() = default;

	/**
	 * @brief Turns all functions into lambda functions so they can be stored as listeners
	 *
	 * @tparam T Class of member function
	 * @param object Pointer to the Object
	 * @param func The function getting added as a listener
	 */
	template<typename T>
	void add(T* object, void(T::* func)(R...));

	/**
	 * @brief Add a raw function to the event, the only way this function can be removed is on clear
	 *
	 * @param function The function to be added to the raw functions
	 */
	void addRaw(std::function<void(R...)> function);

	/**
	 * @brief Searches for the function and removes it from the vector of listeners
	 *
	 * @param object The object owner of the function getting removed
	 * @param func The function getting removed from listeners
	 */
	template<typename T>
	void remove(T* object, void(T::* func)(R...));

	/**
	 * @brief Function that will clear all listeners only works if the event has no owner
	 */
	void clear();

	/**
	 * @brief Call the event with the necessary information to be passed to all Objects.
	 *
	 * @param args The inputs determined by R required to call the Event
	 */
	void call(R... args);

	template<typename T>
	bool contains(T* object, void(T::* func)(R...));

private:
	/**
	 * @brief Vector of lambda functions ready to be called on callEvent()
	 */
	//std::vector<Listener<R...>> m_functions{};
	static unsigned int hash(uint64_t h1, uint64_t h2);

	std::map<uint64_t, std::function<void(R...)>> m_functions{};

	std::vector<std::function<void(R...)>> m_rawFunctions{};
};

template<typename ... R>
template<typename T>
void Event<R...>::add(T *object, void(T::*func)(R...)) {
	int key = hash(reinterpret_cast<uint64_t>(object), reinterpret_cast<uint64_t>(&func));

	if (m_functions.contains(key))
	{
		Log.log("Function already in event... returning");
	}

	std::function<void(R...)> lambda = [object, func](R... args)
    {
        (object->*func)(args...);
    };

    m_functions.insert({key, lambda});
}

template <typename ... R>
void Event<R...>::addRaw(std::function<void(R...)> function)
{
	m_rawFunctions.push_back(function);
}

template<typename ... R>
template<typename T>
void Event<R...>::remove(T *object, void(T::*func)(R...)) {
	int key = hash(reinterpret_cast<uint64_t>(object), reinterpret_cast<uint64_t>(&func));

	if (!m_functions.contains(key))
	{
		Log.log("Function is not in event... returning");
		return;
	}
	m_functions.erase(key);
}

template<typename ... R>
void Event<R...>::clear() {
	m_functions.clear();
	m_rawFunctions.clear();
}

template<typename ... R>
void Event<R...>::call(R... args) {
	for (const auto& it : std::views::values(m_functions))
	{
		it(args...);
	}
	for (const auto& it : m_rawFunctions)
	{
		it(args...);
	}
}

template <typename ... R>
template <typename T>
bool Event<R...>::contains(T* object, void(T::* func)(R...))
{
	auto key = hash(reinterpret_cast<uint64_t>(object), reinterpret_cast<uint64_t>(&func));
	return m_functions.contains(key);
}

template <typename ... R>
unsigned int Event<R...>::hash(uint64_t h1, const uint64_t h2)
{
	h1 ^= h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2);
	return h1;
}
