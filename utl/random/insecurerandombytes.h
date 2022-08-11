#ifndef TENDUKE_UTL_RANDOM_INSECURERANDOMBYTES_H
#define TENDUKE_UTL_RANDOM_INSECURERANDOMBYTES_H

#include "./randombytes.h"
#include <random>


namespace tenduke { namespace utl { namespace random {


/** Default implementation of tenduke::utl::random::RandomBytes.
 *
 */
class InsecureRandomBytes : public tenduke::utl::random::RandomBytes
{
public:
    InsecureRandomBytes();


    // tenduke::utl:random::RandomBytes interface
public:
    virtual void generate(unsigned char *buffer, size_t bufferLength) override;

private:
    std::random_device device;
    std::mt19937 generator;
    std::uniform_int_distribution<std::mt19937::result_type> distribution;
};


}}}


#endif // TENDUKE_UTL_RANDOM_INSECURERANDOMBYTES_H
