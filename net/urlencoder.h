#ifndef URLENCODER_H
#define URLENCODER_H

#include <string>

namespace tenduke { namespace net {


/** Service to URL-encode a string.
 *
 */
class URLEncoder
{
public:
    virtual ~URLEncoder() {}

    /** URL-encodes given string.
     *
     *  @return -
     *  @throws tenduke::net::URLException if the encoding failed
     */
    virtual std::string encode(const std::string &stringToEncode) const = 0;
};


}}

#endif // URLENCODER_H
