#ifndef DEFAULTRANDOMBYTES_H
#define DEFAULTRANDOMBYTES_H

#include "utl/random/randombytes.h"

namespace tenduke { namespace qt { namespace utl {


/** QT-implementation of tenduke::utl::random::RandomBytes.
 *
 */
class QtRandomBytes : public tenduke::utl::random::RandomBytes
{
public:
    QtRandomBytes();

    // tenduke::utl::random::RandomBytes interface
    virtual void generate(
            unsigned char * buffer,
            size_t bufferLength
    ) override;
};


}}}

#endif // DEFAULTRANDOMBYTES_H
