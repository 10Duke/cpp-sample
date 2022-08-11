#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "./httpcall.h"
#include "./httprequest.h"
#include "./httprequestbuilder.h"

#include <memory>

namespace tenduke { namespace http {


/** A simple HTTP-client.
 *
 *  The client is used to build HTTPRequest and then create new call from the request.
 *
 */
class HTTPClient
{
public:
    virtual ~HTTPClient() {}

    /** Starts building request.
     */
    virtual tenduke::http::HTTPRequestBuilder request() const = 0;

    /** Constructs HTTPCall from given HTTPRequest.
     *
     *  @param request -
     *  @return -
     */
    virtual std::unique_ptr<tenduke::http::HTTPCall> call(std::shared_ptr<const tenduke::http::HTTPRequest> call) const = 0;
};


}}

#endif // HTTPCLIENT_H
