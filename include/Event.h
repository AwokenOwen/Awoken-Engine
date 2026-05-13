//
// Created by AwokenOwen on 4/13/26.

#pragma once
#include <functional>
#include <vector>

/**
 * @brief A macro for adding and remove from specific events. Adding this wrapper in the public space allows events to be private but publicly added and removed from
 *
 * @param EventName The name of the event variable
 * @param ... the types of information being passed by the event
 */
#define EVENT_ACCESSORS(EventName, ...)                                          \
	template<typename T>                                                         \
	void addTo_##EventName(T* object, void(T::* func)(__VA_ARGS__)) {              \
		EventName.add(object, func);                                             \
	}                                                                            \
	template<typename T>                                                         \
	void removeFrom_##EventName(T* object, void(T::* func)(__VA_ARGS__)) {           \
		EventName.remove(object, func);                                          \
	}

template<typename... R>
struct Listener {
	int m_hash{};
	std::function<void(R...)> m_function{};

	bool operator==(Listener& other) {
		return m_hash == other.m_hash;
	}
};

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
	void clearEvent();

	/**
	 * @brief Call the event with the necessary information to be passed to all Objects.
	 *
	 * @param args The inputs determined by R required to call the Event
	 */
	void callEvent(R... args);

private:
	/**
	 * @brief Vector of lambda functions ready to be called on callEvent()
	 */
	std::vector<Listener<R...>> m_functions{};
};

template<typename ... R>
template<typename T>
void Event<R...>::add(T *object, void(T::*func)(R...)) {
    std::function<void(R...)> lambda = [object, func](R... args)
    {
        (object->*func)(args...);
    };

	int hash = *reinterpret_cast<int*>(object);
	hash *= *reinterpret_cast<int*>(&func);

    Listener<R...> listener{hash, lambda};

    m_functions.emplace_back(listener);
}

template<typename ... R>
template<typename T>
void Event<R...>::remove(T *object, void(T::*func)(R...)) {
	int hash = *reinterpret_cast<int*>(object);
	hash *= *reinterpret_cast<int*>(&func);

    std::erase_if(m_functions, [hash](const Listener<R...>& listener) {
        return listener.m_hash == hash;
    });
}

template<typename ... R>
void Event<R...>::clearEvent() {
	m_functions.clear();
}

template<typename ... R>
void Event<R...>::callEvent(R... args) {
	for (auto f : m_functions) {
		f.m_function(args...);
	}
}