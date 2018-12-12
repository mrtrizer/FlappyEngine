#pragma once

namespace flappy {

class IEvent {
public:
    IEvent() = default;
    IEvent(const IEvent&) = default;
    IEvent& operator=(const IEvent&) & = default;
    IEvent(IEvent&&) = default;
    IEvent& operator=(IEvent&&) & = default;
    virtual ~IEvent() = default;
};

} // flappy
