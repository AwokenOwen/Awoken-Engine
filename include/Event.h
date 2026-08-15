//
// Created by AwokenOwen on 4/13/26.

#pragma once
#include <functional>
#include <map>
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

	/**
	 * @brief Function to tell if a function is already added to the event
	 *
	 * @tparam T The object type
	 * @param object Pointer to the object that owns the function getting tested
	 * @param func Pointer to the function being tested
	 * @return Whether the has of the two pointers is in function map
	 */
	template<typename T>
	bool contains(T* object, void(T::* func)(R...));

private:

	/**
	 * @brief Takes in two uint64_ts and produces a hash value for them (used to get a hash for the object and function pointer)
	 *
	 * @param h1 Value 1
	 * @param h2 Value 2
	 * @return The hash value of the two uint64s
	 */
	static uint64_t hash(uint64_t h1, uint64_t h2);

	/**
	 * @brief Unordered map storing all the hash values with the position in the vector of the lambda
	 */
	std::unordered_map<uint64_t, unsigned int> m_functionMap{};
	/**
	 * @brief Vector of lambda functions ready to be called on callEvent()
	 */
	std::vector<std::function<void(R...)>> m_functions{};
	/**
	 * @brief Vector of keys in the function map parallel to m_functions
	 */
	std::vector<uint64_t> m_functionKeys{};
};

template<typename ... R>
template<typename T>
void Event<R...>::add(T *object, void(T::*func)(R...)) {
	// Get the hash of the object pointer and the function pointer
	const auto key = hash(reinterpret_cast<uint64_t>(object), reinterpret_cast<uint64_t>(&func));

	// See if the has is already in the map AKA see if the function of this object instance is already in the function list
	if (m_functionMap.contains(key))
	{
		// Log Warning, not logging as error because it's only an error if the hash of different pairs of pointers creates the same hash
		Log.logWarning("Function already in event... returning");
		return;
	}

	// Create lambda function from the two pointers
	std::function<void(R...)> lambda = [object, func](R... args)
    {
        (object->*func)(args...);
    };

	// Insert hash value with the position of the function in the vector of lambda functions
    m_functionMap.insert({key, static_cast<unsigned int>(m_functions.size())});
	m_functions.push_back(lambda);
	m_functionKeys.push_back(key);
}

template <typename ... R>
void Event<R...>::addRaw(std::function<void(R...)> function)
{
	// Add a lambda function directly to the function list
	m_functions.push_back(function);
}

template<typename ... R>
template<typename T>
void Event<R...>::remove(T *object, void(T::*func)(R...)) {
	// Get the hash of the object pointer and the function pointer
	const auto key = hash(reinterpret_cast<uint64_t>(object), reinterpret_cast<uint64_t>(&func));

	// Check if the hash is not in the function map AKA there is no function to remove
	auto it = m_functionMap.find(key);
	if (it == m_functionMap.end())
	{
		// Log Warning, Not logging as error because it's only an error if the pointer's no longer match which shouldn't happen
		Log.logWarning("Function is not in event... returning");
		return;
	}

	// Get the index of the function in the function list
	auto index = it->second;
	m_functionMap.erase(it);

	// Grab the last index to make sure the function being removed isn't already at the end
	auto lastIndex = static_cast<unsigned int>(m_functions.size() - 1);
	if (index != lastIndex)
	{
		// Swap the function and the key to the end
		std::swap(m_functions[index], m_functions.back());
		std::swap(m_functionKeys[index], m_functionKeys.back());
		// Whatever moved into `index` needs its map entry updated
		m_functionMap[m_functionKeys[index]] = index;
	}

	// Pop the end of the functions and keys to remove functions at constant time
	m_functions.pop_back();
	m_functionKeys.pop_back();
}

template<typename ... R>
void Event<R...>::clear() {
	// Clear the vectors and unordered map
	m_functions.clear();
	m_functionKeys.clear();
	m_functionMap.clear();
}

template<typename ... R>
void Event<R...>::call(R... args) {
	// Loop through the functions
	for (const auto& func : m_functions)
	{
		// Call each lambda function which will call the member functions (even private ones)
		func(args...);
	}
}

template <typename ... R>
template <typename T>
bool Event<R...>::contains(T* object, void(T::* func)(R...))
{
	// Get the hash
	const auto key = hash(reinterpret_cast<uint64_t>(object), reinterpret_cast<uint64_t>(&func));
	// Check if the key is in the map and return it
	return m_functionMap.contains(key);
}

template <typename ... R>
uint64_t Event<R...>::hash(uint64_t h1, const uint64_t h2)
{
	// The hash function does a bitwise exclusive OR with addition of h2, a magic constant, bit shifted h1 and bit shifted h2
	h1 ^= h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2);
	// return the result as the hash function
	return h1;
}
