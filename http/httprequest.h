#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <map>
#include <string>

namespace tenduke { namespace http {

// Default connection time out in milliseconds.
const long DEFAULT_CONNECTION_TIMEOUT_MS = 30L * 1000L;

// Default total time out in milliseconds
const long DEFAULT_TIMEOUT_MS = 0L; // 0 = does not time out

/** HTTP-request, which contains all necessary details to construct the request.
 *
 */
class HTTPRequest
{
public:
    HTTPRequest(
            const std::string method,
            const std::string url,
            const std::multimap<std::string, std::string> headers = std::multimap<std::string, std::string>(),
            const std::string messageBody = "",
            long connectionTimeoutMs = DEFAULT_CONNECTION_TIMEOUT_MS,
            long timeoutMs = DEFAULT_TIMEOUT_MS
    ) : method(method), messageBody(messageBody), url(url), headers(headers),
        connectionTimeoutMs(connectionTimeoutMs), timeoutMs(timeoutMs)
    {}

    const std::string & getMessageBody() const {return messageBody;}
    const std::string & getMethod() const {return method;}
    const std::string & getUrl() const {return url;}
    const std::multimap<std::string, std::string> & getHeaders() const {return headers;}
    long getConnectionTimeoutMs() const {return connectionTimeoutMs;}
    long getTimeoutMs() const {return timeoutMs;}

    bool hasBody() const {return !messageBody.empty();}

private:
    const std::string method;
    const std::string messageBody;
    const std::string url;
    const std::multimap<std::string, std::string> headers;
    const long connectionTimeoutMs;
    const long timeoutMs;
};


}}

#endif // HTTPREQUEST_H
