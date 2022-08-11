#ifndef RANDOMBYTES_H
#define RANDOMBYTES_H

#include <stddef.h>

namespace tenduke { namespace utl { namespace random {


/** Generates random bytes.
 *
 */
class RandomBytes
{
public:
    virtual ~RandomBytes() {}

    /** Fills the given buffer with random bytes.
     *
     *  @param buffer the buffer
     *  @param bufferLenght size of the buffer in bytes
     */
    virtual void generate(
            unsigned char * buffer,
            size_t bufferLength
    ) = 0;
};


}}}

#endif // RANDOMBYTES_H
