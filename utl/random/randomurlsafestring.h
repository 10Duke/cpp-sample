#ifndef RANDOMURLSAFESTRING_H
#define RANDOMURLSAFESTRING_H

#include "./randombytes.h"

#include <memory>
#include <stddef.h>
#include <string>

namespace tenduke { namespace utl { namespace random {


/** Generates string of random URL-safe characters.
 *
 */
class RandomUrlSafeString
{
public:
    RandomUrlSafeString(const std::shared_ptr<RandomBytes> randomBytes)
        : randomBytes(randomBytes)
    {}

    /** Generates random string.
     *
     *  @param length num characters to generate
     *  @return random string of length
     */
    std::string generate(size_t length);

private:
    const std::shared_ptr<RandomBytes> randomBytes;
};


}}}

#endif // RANDOMURLSAFESTRING_H
