#ifndef DEFAULTLICENSINGCLIENT_H
#define DEFAULTLICENSINGCLIENT_H

#include "./licensingclient.h"
#include "./licensingconfiguration.h"

#include "http/httpclient.h"
#include "json/jsonparser.h"

namespace tenduke { namespace licensing {


/** Default implementation of tenduke::licensing::LicensingClient.
 *
 */
class DefaultLicensingClient : public LicensingClient
{
public:
    DefaultLicensingClient(
            std::shared_ptr<LicensingConfiguration> configuration,
            std::shared_ptr<tenduke::http::HTTPClient> httpClient,
            std::shared_ptr<tenduke::json::JSONParser> jsonParser
    );
    virtual ~DefaultLicensingClient() {}

    // tenduke::licensing::LicensingClient interface
    virtual std::unique_ptr<tenduke::licensing::LicenseCheckoutRequest> checkout(std::unique_ptr<const tenduke::licensing::LicenseCheckoutParameters> parameters);

private:
    const std::shared_ptr<const LicensingConfiguration> configuration;
    const std::shared_ptr<tenduke::http::HTTPClient> httpClient;
    const std::shared_ptr<tenduke::json::JSONParser> jsonParser;
};


}}

#endif // DEFAULTLICENSINGCLIENT_H
