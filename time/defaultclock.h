#ifndef DEFAULTCLOCK_H
#define DEFAULTCLOCK_H

#include "./clock.h"

namespace tenduke { namespace time {


/** Default implementation tenduke::time::Clock.
 *
 */
class DefaultClock : public Clock
{
public:
    DefaultClock() {}

    // tenduke::time::Clock interface
    virtual std::int64_t epochSeconds() const noexcept override;
};


}}

#endif // DEFAULTCLOCK_H
