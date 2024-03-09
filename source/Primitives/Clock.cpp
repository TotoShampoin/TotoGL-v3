#include "TotoGL/Primitives/Clock.hpp"

#include <chrono>

namespace TotoGL {

Seconds Clock::getTime() {
    using SystemSecond = std::chrono::duration<double, std::ratio<1, 1>>;
    using std::chrono::duration_cast;

    auto now = timeNow();
    return duration_cast<SystemSecond>(now - _start).count();
}

Seconds Clock::getDeltaTime() {
    using SystemSecond = std::chrono::duration<double, std::ratio<1, 1>>;
    using std::chrono::duration_cast;

    static auto last = timeNow();
    auto now = timeNow();
    auto delta = duration_cast<SystemSecond>(now - last).count();
    last = now;
    return delta;
}

std::chrono::nanoseconds Clock::timeNow() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock().now().time_since_epoch());
}

} // namespace TotoGL
