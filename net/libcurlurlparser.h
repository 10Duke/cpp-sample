#ifndef LIBCURLURLPARSER_H
#define LIBCURLURLPARSER_H

#include "urlparser.h"

namespace tenduke { namespace net { namespace curl {


/** Libcurl implementation of tenduke::net::URLParser.
 *
 */
class LibCurlURLParser : public tenduke::net::URLParser
{
public:
    /** Constructs new instance.
     *
     *  @param lenient parse the URL leniently (allow URL scheme not recognized by libcurl)
     */
    LibCurlURLParser(bool lenient = true);
    virtual ~LibCurlURLParser() {}

    // tenduke::net::URLParser interface
public:
    virtual std::unique_ptr<tenduke::net::URL> from(const std::string &url) const override;

private:
    const bool lenient;
};


}}}

#endif // LIBCURLURLPARSER_H
