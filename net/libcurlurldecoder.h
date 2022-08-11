#ifndef LIBCURLURLDECODER_H
#define LIBCURLURLDECODER_H

#include "./urldecoder.h"

#include <curl/curl.h>

namespace tenduke { namespace net { namespace curl {


/** Libcurl implementation of tenduke::net::URLDecoder.
 *
 */
class LibCurlURLDecoder : public tenduke::net::URLDecoder
{
public:
    virtual ~LibCurlURLDecoder() {}

    // tenduke::net::URLDecoder interface
    virtual std::string decode(const char * string, const size_t len) const override;
};


/** Decodes given string using given CURL-handle.
 *
 *  This assumes that the encoded value decodes to e.g. UTF-8 encoded string.
 *
 *  @param curl the CURL-handle
 *  @param encodedString URL-encoded string
 *  @param len length of the URL-encoded String
 *
 *  @return decoded string
 *
 *  @throws tenduke::net::URLException if the decoding failed
 */
std::string urlDecode(
    CURL * curl,
    const char *encodedString,
    const size_t len
);


}}}

#endif // LIBCURLURLDECODER_H
