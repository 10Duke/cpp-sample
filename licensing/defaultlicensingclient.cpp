#include "./defaultlicensingclient.h"
#include "./defaultlicensecheckoutrequest.h"

namespace licensing = tenduke::licensing;

licensing::DefaultLicensingClient::DefaultLicensingClient(
        std::shared_ptr<licensing::LicensingConfiguration> configuration,
        std::shared_ptr<const tenduke::http::HTTPClient> httpClient,
        std::shared_ptr<const tenduke::jwt::JWTParser> jwtParser
) : configuration(configuration), httpClient(httpClient), jwtParser(jwtParser)
{
}

std::unique_ptr<licensing::LicenseCheckoutRequest> licensing::DefaultLicensingClient::checkout(std::unique_ptr<const licensing::LicenseCheckoutParameters> parameters)
{
    return std::unique_ptr<licensing::LicenseCheckoutRequest>(new licensing::DefaultLicenseCheckoutRequest(
        jwtParser,
        httpClient,
        configuration,
        std::move(parameters)
    ));
}
