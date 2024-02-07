#pragma once

#include <chrono>

namespace TotoGL {

using Seconds = float;

class Clock {
public:
    Clock() = default;
    ~Clock() = default;

    Seconds getTime() {
        using SystemSecond = std::chrono::duration<double, std::ratio<1, 1>>;
        using std::chrono::duration_cast;

        auto now = timeNow();
        return duration_cast<SystemSecond>(now - _start).count();
    }
    Seconds getDeltaTime() {
        using SystemSecond = std::chrono::duration<double, std::ratio<1, 1>>;
        using std::chrono::duration_cast;

        static auto last = timeNow();
        auto now = timeNow();
        auto delta = duration_cast<SystemSecond>(now - last).count();
        last = now;
        return delta;
    }

private:
    std::chrono::nanoseconds _start { timeNow() };

    static std::chrono::nanoseconds timeNow() {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock().now().time_since_epoch());
    }
};

} // namespace TotoGL
