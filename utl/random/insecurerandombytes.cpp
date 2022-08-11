#include "insecurerandombytes.h"

namespace xdrandom = tenduke::utl::random;

xdrandom::InsecureRandomBytes::InsecureRandomBytes()
    : device(),
      generator(device()),
      distribution(0, 255)
{
}

void xdrandom::InsecureRandomBytes::generate(
        unsigned char *buffer,
        size_t bufferLength
)
{
    while (bufferLength > 0) {
        *buffer++ = distribution(generator) & 0xff;
        bufferLength--;
    }
}
