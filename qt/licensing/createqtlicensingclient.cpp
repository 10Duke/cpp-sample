#include "./createqtlicensingclient.h"

#include "licensing/defaultlicensingclient.h"
#include "qt/http/createqthttpclient.h"
#include "qt/json/qtjsonparser.h"

namespace qtjson = tenduke::qt::json;
namespace qtlic = tenduke::qt::licensing;
namespace qthttp = tenduke::qt::http;
namespace xdhttp = tenduke::http;
namespace xdjson = tenduke::json;
namespace xdlic = tenduke::licensing;

std::unique_ptr<qtlic::QtLicensingClient> qtlic::createQtLicensingClient(
        const qtlic::QtLicensingConfiguration &configuration,
        const std::string & accessToken,
        std::shared_ptr<xdhttp::HTTPClient> httpClient,
        std::shared_ptr<tenduke::json::JSONParser> jsonParser
)
{
    std::shared_ptr<xdlic::LicensingConfiguration> config(new xdlic::LicensingConfiguration(
        accessToken,
        configuration.getEndpoint().toString().toStdString()
    ));

    std::shared_ptr<xdlic::LicensingClient> licensingClient(new xdlic::DefaultLicensingClient(
        config,
        httpClient,
        jsonParser
    ));

    return std::unique_ptr<qtlic::QtLicensingClient>(new qtlic::QtLicensingClient(
        config,
        licensingClient,
        configuration.getHardwareId().toStdString()
    ));
}

std::unique_ptr<qtlic::QtLicensingClient> qtlic::createQtLicensingClient(
        const qtlic::QtLicensingConfiguration &configuration,
        const std::string & accessToken,
        std::shared_ptr<xdhttp::HTTPClient> httpClient
)
{
    return qtlic::createQtLicensingClient(
        configuration,
        accessToken,
        httpClient,
        std::shared_ptr<xdjson::JSONParser>(new qtjson::QtJSONParser())
    );
}

std::unique_ptr<qtlic::QtLicensingClient> qtlic::createQtLicensingClient(
        const qtlic::QtLicensingConfiguration &configuration,
        const std::string & accessToken,
        std::shared_ptr<QNetworkAccessManager> networkAccessManager
)
{
    return qtlic::createQtLicensingClient(
        configuration,
        accessToken,
        qthttp::createQtHTTPClient(networkAccessManager)
    );
}
