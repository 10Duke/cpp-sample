#include "randomurlsafestring.h"

#include <math.h>
#include <memory>

static std::string allowedCharacters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-._~";

namespace rnd = tenduke::utl::random;

std::string rnd::RandomUrlSafeString::generate(size_t length)
{
    std::string result;
    result.reserve(length);

    std::unique_ptr<unsigned char> bytes(new unsigned char[length]);
    randomBytes->generate(bytes.get(), length);

    for (size_t i = 0; i < length; i++) {
        result += allowedCharacters[bytes.get()[i] & 0x3f];
    }

    return result;
}
