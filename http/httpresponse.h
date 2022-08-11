#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include "utl/binarydata.h"

#include <map>
#include <memory>
#include <string>

namespace tenduke { namespace http {

/** A HTTP Response.
 */
class HTTPResponse
{
public:
    HTTPResponse(
            int statusCode,
            std::multimap<std::string, std::string> headers,
            std::unique_ptr<tenduke::utl::BinaryData> data
    );
    virtual ~HTTPResponse() {}

    virtual std::string getPayloadAsString();
    virtual size_t getPayloadLength();

    /** Returns the HTTP-status code.
     *
     *  @return the HTTP-status code
     */
    int getStatusCode() {return statusCode;}

    /** Checks if the request is successful based on the HTTP status code.
     *
     *  @return true if the request is "successful" based on HTTP status code.
     */
    bool isSuccessful();

private:
    const int statusCode;
    const std::multimap<std::string, std::string> headers;
    const std::unique_ptr<tenduke::utl::BinaryData> data;
};

}}

#endif // HTTPRESPONSE_H
