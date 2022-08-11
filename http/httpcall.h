#ifndef HTTPCALL_H
#define HTTPCALL_H

#include "./httpresponse.h"

#include <memory>

namespace tenduke { namespace http {


/** HTTP-call which executes the request and returns HTTPResponse.
 *
 */
class HTTPCall
{
public:
    virtual ~HTTPCall() {}

    /** Executes the call synchronously.
     *
     *  NOTE: This is blocking call.
     *
     *  @return HTTP-response.
     *
     *  @throws tenduke::net::NetworkingException (or subclasses) when the call fails for various networking reasons like
     *          "no such host", "network timeout", "networking connection broken" and so on. Some of these are transient
     *          and thus re-triable, some indicate configuration error.
     *  @throws tenduke::net::HTTPRequestException when the call fails for various reasons, see documentation for the
     *          exception for details. **NOTE** HTTPCall does not throw exception if the request technically succeeds, but
     *          the server returns HTTP status code >= 400. It is responsibility of the caller to interpret the status code
     *          and act accordingly.
     */
    virtual std::unique_ptr<tenduke::http::HTTPResponse> execute() = 0;
};


}}

#endif // HTTPCALL_H
