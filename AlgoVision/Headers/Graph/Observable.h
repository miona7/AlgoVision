#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <functional>
#include <map>

template <typename T> class Observable {
public:
    using Observer = std::function<void(T&)>;

    unsigned addObserver(Observer observer) {
        unsigned id     = ++m_id;
        m_observers[id] = std::move(observer);
        return id;
    }

    void removeObserver(unsigned id) {
        m_observers.erase(id);
    }

protected:
    void notifyObservers(T& subject) {
        for(auto& [_, observer]: m_observers) {
            observer(subject);
        }
    }

private:
    unsigned                     m_id = 0;
    std::map<unsigned, Observer> m_observers;
};

#endif // OBSERVABLE_H
