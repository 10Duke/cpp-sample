#ifndef URLPARSER_H
#define URLPARSER_H

#include "./url.h"

#include <memory>
#include <string>

namespace tenduke { namespace net {


/** A service to parse URLs from strings.
 *
 */
class URLParser
{
public:
    virtual ~URLParser() {}

    /** Parses URL.
     *
     *  @throws tenduke::net::URLException (or subclasses) if parsing the URL failed
     */
    virtual std::unique_ptr<URL> from(const std::string &urlToParse) const = 0;
};


}}


#endif // URLPARSER_H
