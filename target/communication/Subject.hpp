#pragma once

#include <vector>
#include <algorithm>
#include "Observer.hpp"


template <class T>
class Subject
{
public:

    Subject() = default;

    virtual ~Subject() = default;

    void attachObserver (Observer<T> &observer)
    {
        mObservers.push_back(&observer);
    }

    void detachObserver(Observer<T> &observer)
    {
        auto iterator = std::find(mObservers.begin(), mObservers.end(), observer);

        if (iterator != mObservers.end()) {
            mObservers.erase(iterator);
        }
    }

    void notifyObservers (int event_id, T value)
    {
        typename std::vector<Observer<T> *>::iterator it;

        for (it=mObservers.begin(); it!=mObservers.end(); it++) {
            (*it)->update(event_id, value);
        }
    }

private:

    std::vector<Observer<T> *> mObservers;
};
