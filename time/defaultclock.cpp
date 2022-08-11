#include "./defaultclock.h"

#include <chrono>

namespace xdtime = tenduke::time;

std::int64_t xdtime::DefaultClock::epochSeconds() const
noexcept
{
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
