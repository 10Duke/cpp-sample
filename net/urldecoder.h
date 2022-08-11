#ifndef URLDECODER_H
#define URLDECODER_H

#include <string>

namespace tenduke { namespace net {


/** Service to URL-decode a string.
 *
 */
class URLDecoder
{
public:
    virtual ~URLDecoder() {}

    /** URL-decodes given string.
     *
     *  This assumes that the encoded value decodes to e.g. UTF-8 encoded string.
     *  Technically the decoded data could be binary data.
     *
     *  @return -
     *  @throws tenduke::net::URLException if the decoding failed
     */
    virtual std::string decode(const char * string, const size_t len) const = 0;
};


}}


#endif // URLDECODER_H
