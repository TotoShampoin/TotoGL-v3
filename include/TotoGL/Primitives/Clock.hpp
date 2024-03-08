#pragma once

#include <chrono>

namespace TotoGL {

using Seconds = float;

class Clock {
public:
    Clock() = default;
    ~Clock() = default;

    Seconds getTime();
    Seconds getDeltaTime();

private:
    std::chrono::nanoseconds _start { timeNow() };

    static std::chrono::nanoseconds timeNow();
};

} // namespace TotoGL
