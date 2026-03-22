#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <map>
#include <functional>

/**
 * @class Observable
 * @brief Base class implementing the Observer pattern for any subject type.
 *
 * This template class allows objects to maintain a list of observers that
 * are notified when the object's state changes. For example, Node and Edge
 * inherit from Observable so that their corresponding NodeItem and EdgeItem
 * can update their visual representation automatically.
 *
 * @tparam T Type of the observed subject. Observers receive a reference to this type.
 */

template <typename T> class Observable {
public:
    /// Type of the observer function: takes a reference to the subject
    using Observer = std::function<void(T&)>;

    /**
     * @brief Adds a new observer.
     * @param observer The function to call when the subject changes.
     * @return A unique ID for the observer, which can be used for removal.
     */
    unsigned addObserver(Observer observer) {
        unsigned id     = ++m_id;
        m_observers[id] = std::move(observer);
        return id;
    }

    /**
     * @brief Removes an observer by its unique ID.
     * @param id The ID of the observer to remove.
     */
    void removeObserver(unsigned id) {
        m_observers.erase(id);
    }

protected:
    /**
     * @brief Notifies all registered observers of a state change.
     * @param subject The subject whose state has changed.
     *
     * This method copies the observer map before iterating to allow safe removal
     * of observers during notification.
     */
    void notifyObservers(T& subject) {
        auto observersCopy = m_observers;
        for(auto& [_, observer]: observersCopy) {
            if(observer) {
                observer(subject);
            }
        }
    }

private:
    unsigned                     m_id {0};    /**< Counter for assigning unique observer IDs */
    std::map<unsigned, Observer> m_observers; /**< Map of observer ID to observer function */
};

#endif // OBSERVABLE_H
