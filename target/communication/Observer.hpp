#pragma once

template <class T>
class Observer
{
public:

    Observer() = default;

    virtual ~Observer() = default;

    virtual void update(int event_id, T value) = 0;
};

