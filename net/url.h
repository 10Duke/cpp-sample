#ifndef URL_H
#define URL_H

#include <map>
#include <string>

namespace tenduke { namespace net {


/** Represents URL.
 *
 */
class URL
{
public:
    virtual ~URL() {}

    /** Returns the query parameters, URL-decoded.
     *
     *  @return -
     *
     *  @throws tenduke::net::URLException in case something failed
     */
    virtual std::multimap<std::string, std::string> getQueryParameters() const = 0;
};


}}


#endif // URL_H
