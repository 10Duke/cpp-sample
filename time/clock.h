#ifndef CLOCK_H
#define CLOCK_H

#include <cstdint>

namespace tenduke { namespace time {


/** A clock-service to generate timestamps.
 *
 */
class Clock
{
public:
    virtual ~Clock() {}

    /** Returns current time as seconds from epoch.
     *
     *  @return -
     */
    virtual std::int64_t epochSeconds() const noexcept = 0;
};


}}

#endif // CLOCK_H
