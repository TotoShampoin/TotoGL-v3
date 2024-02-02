#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace TotoGL {

template <
    typename EventName = std::string,
    typename EventValue = int>
class EventListener {
    using EventCallback = std::function<void(EventValue)>;

public:
    void on(const EventName& name, const EventCallback& callback) {
        _listeners[name].push_back(callback);
    }
    void emit(const EventName& name, const EventValue& value) {
        if (!_listeners.contains(name))
            return;
        for (auto& callback : _listeners.at(name)) {
            callback(value);
        }
    }

private:
    std::map<EventName, std::vector<EventCallback>> _listeners;
};

} // namespace TotoGL
