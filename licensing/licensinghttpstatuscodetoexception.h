#ifndef TENDUKE_LICENSING_LICENSINGHTTPSTATUSCODETOEXCEPTION_H
#define TENDUKE_LICENSING_LICENSINGHTTPSTATUSCODETOEXCEPTION_H

#include "http/httpstatuscodetoexception.h"

namespace tenduke { namespace licensing {


/** Licensing-specific HTTPStatusCode-To-Exception -mapper.
 */
class LicensingHTTPStatusCodeToException : public tenduke::http::HTTPStatusCodeToException
{
protected:
    virtual void from403() const override;
};


}}

#endif // TENDUKE_LICENSING_LICENSINGHTTPSTATUSCODETOEXCEPTION_H
