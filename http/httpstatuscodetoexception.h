#ifndef TENDUKE_HTTP_HTTPSTATUSCODETOEXCEPTION_H
#define TENDUKE_HTTP_HTTPSTATUSCODETOEXCEPTION_H

namespace tenduke { namespace http {


/** Utility service to throw an HTTPException based on HTTP status-code.
 *
 *  This service generates default exceptions for HTTP status codes and can be
 *  specialized if needed.
 *
 *  Not all HTTP-status codes implemented yet.
 */
class HTTPStatusCodeToException
{
public:
    virtual void fromHTTPStatus(int httpStatusCode) const;

protected:
    virtual void from4xx(int httpStatusCode) const;
    virtual void from400() const;
    virtual void from401() const;
    virtual void from403() const;
    virtual void from404() const;
    virtual void from4xxOther(int httpStatusCode) const;
    virtual void from5xx(int httpStatusCode) const;
};

}}


#endif // TENDUKE_HTTP_HTTPSTATUSCODETOEXCEPTION_H
