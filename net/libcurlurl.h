#ifndef LIBCURLURL_H
#define LIBCURLURL_H

#include "./url.h"

#include <curl/curl.h>

namespace tenduke { namespace net { namespace curl {


/** Libcurl-based implementation of tenduke::net::URL.
 */
class LibcurlURL : public tenduke::net::URL
{
public:
    /** Constructs new instance.
     *
     *  @param lenient accept URL-scheme not recognized by libcurl.
     *
     *  @throws std::bad_alloc if unable to initialize libcurl
     */
    LibcurlURL(bool lenient = true);
    virtual ~LibcurlURL();

    // tenduke::net::URL interface
    virtual std::multimap<std::string, std::string> getQueryParameters() const override;

public:
    /** Parses given URL into this instance.
     *
     *  @param url -
     *
     *  @throws tenduke::net::MalformedURLException if URL malformed
     *  @throws tenduke::net::URLException (or other subclasses) for other parsing reasongs
     */
    virtual void parse(const std::string &url);

private:
    bool lenient;
    CURLU *curlHandle;
};


/** Utility function to parse query string into a map.
 *
 *  @param queryString the query string
 *
 *  @return query parameters
 *
 *  @throws tenduke::net::URLException if libcurl reported errors
 */
std::multimap<std::string, std::string> parseQueryString(const char * const queryString);

}}}

#endif // LIBCURLURL_H
