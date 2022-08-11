#include "./defaultlicensingclient.h"
#include "./defaultlicensecheckoutrequest.h"

namespace licensing = tenduke::licensing;

licensing::DefaultLicensingClient::DefaultLicensingClient(
        std::shared_ptr<licensing::LicensingConfiguration> configuration,
        std::shared_ptr<tenduke::http::HTTPClient> httpClient,
        std::shared_ptr<tenduke::json::JSONParser> jsonParser
) : configuration(configuration), httpClient(httpClient), jsonParser(jsonParser)
{
}

std::unique_ptr<licensing::LicenseCheckoutRequest> licensing::DefaultLicensingClient::checkout(std::unique_ptr<const licensing::LicenseCheckoutParameters> parameters)
{
    return std::unique_ptr<licensing::LicenseCheckoutRequest>(new licensing::DefaultLicenseCheckoutRequest(
        jsonParser,
        httpClient,
        configuration,
        std::move(parameters)
    ));
}
