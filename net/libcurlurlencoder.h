#ifndef LIBCURLURLENCODER_H
#define LIBCURLURLENCODER_H

#include "./urlencoder.h"

namespace tenduke { namespace net { namespace curl {


/** Libcurl implementation of tenduke::net::URLEncoder.
 *
 */
class LibCurlURLEncoder : public tenduke::net::URLEncoder
{
public:
    virtual ~LibCurlURLEncoder() {}

    // tenduke::net::URLEncoder interface
    virtual std::string encode(const std::string &stringToEncode) const;
};


}}}

#endif // LIBCURLURLENCODER_H
