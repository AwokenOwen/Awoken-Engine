//
// Created by AwokenOwen on 4/13/26.
//

#pragma once
#include <functional>
#include <vector>

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
	Event();
	template<typename T>
	explicit Event(T* owner = nullptr);

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
	 * @brief Function that will clear all listeners if caller hash and owner hash match
	 *
	 * @tparam T The type of the caller
	 * @param caller pointer to the caller object
	 */
	template<typename T>
	void clearEvent(T* caller);

	/**
	 * @brief Call the event with the necessary information to be passed to all Objects. Only works if there is no owner
	 *
	 * @param args The inputs determined by R required to call the Event
	 */
	void callEvent(R... args);
	/**
	 * @brief Call the event with the necessary information to be passed to all Objects. Will do nothing if caller hash is not equal to owner hash
	 *
	 *@tparam T the type of the caller
	 * @param args The inputs determined by R required to call the Event
	 * @param caller pointer to the object that calls the event
	 */
	template<typename T>
	void callEvent(R... args, T* caller);

private:
	/**
	 * @brief Vector of lambda functions ready to be called on callEvent()
	 */
	std::vector<Listener<R...>> m_functions{};

	size_t m_ownerHash{};
};

template<typename ... R>
Event<R...>::Event() {
    m_ownerHash = -1;
}

template<typename ... R>
template<typename T>
Event<R...>::Event(T *owner) {
    m_ownerHash = *reinterpret_cast<int*>(owner);
}

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
    if (m_ownerHash == -1) {
        m_functions.clear();
    }
}

template<typename ... R>
template<typename T>
void Event<R...>::clearEvent(T *caller) {
	if (const int hash = *reinterpret_cast<int*>(caller); hash == m_ownerHash) {
        m_functions.clear();
    }
}

template<typename ... R>
void Event<R...>::callEvent(R... args) {
    if (m_ownerHash == -1) {
        for (auto f : m_functions) {
            f.m_function(args...);
        }
    }
}

template<typename ... R>
template<typename T>
void Event<R...>::callEvent(R... args, T *caller) {
	if (const int hash = *reinterpret_cast<int*>(caller); hash == m_ownerHash) {
		for (auto f : m_functions) {
			f.m_function(args...);
		}
	}
}