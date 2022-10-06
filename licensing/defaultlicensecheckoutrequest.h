#ifndef DEFAULTLICENSECHECKOUTREQUEST_H
#define DEFAULTLICENSECHECKOUTREQUEST_H

#include "./licensecheckoutparameters.h"
#include "./licensecheckoutrequest.h"
#include "./licensecheckoutresponse.h"
#include "./licensingconfiguration.h"

#include "http/httpclient.h"
#include "http/httpstatuscodetoexception.h"
#include "jwt/jwtparser.h"

#include <memory>
#include <string>


namespace tenduke { namespace licensing {


/** Default implementation of tenduke::licensing::LicenseCheckoutRequest.
 *
 */
class DefaultLicenseCheckoutRequest : public LicenseCheckoutRequest
{
public:
    DefaultLicenseCheckoutRequest(
        std::shared_ptr<const tenduke::jwt::JWTParser> jwtParser,
        std::shared_ptr<const tenduke::http::HTTPClient> httpClient,
        std::shared_ptr<const tenduke::licensing::LicensingConfiguration> config,
        std::unique_ptr<const tenduke::licensing::LicenseCheckoutParameters> params
    );
    virtual ~DefaultLicenseCheckoutRequest();

    // tenduke::licensing::LicenseCheckoutRequest interface
    virtual std::unique_ptr<tenduke::licensing::LicenseCheckoutResponse> execute();

protected:
    virtual std::unique_ptr<tenduke::licensing::LicenseCheckoutResponse> parseResponsePayload(const std::string &payload);

private:
    const std::shared_ptr<const tenduke::jwt::JWTParser> parseJWT;
    const std::shared_ptr<const tenduke::http::HTTPClient> http;
    const std::shared_ptr<const tenduke::licensing::LicensingConfiguration> config;
    const std::unique_ptr<const tenduke::licensing::LicenseCheckoutParameters> params;
    const std::shared_ptr<const tenduke::http::HTTPStatusCodeToException> throwException;
};


}}

#endif // DEFAULTLICENSECHECKOUTREQUEST_H
