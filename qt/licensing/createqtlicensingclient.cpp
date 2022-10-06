#include "./createqtlicensingclient.h"

#include "licensing/defaultlicensingclient.h"
#include "qt/http/createqthttpclient.h"
#include "qt/jwt/createqtjwtparser.h"

namespace qthttp = tenduke::qt::http;
namespace qtlic = tenduke::qt::licensing;
namespace qtjwt = tenduke::qt::jwt;
namespace xdhttp = tenduke::http;
namespace xdjwt = tenduke::jwt;
namespace xdlic = tenduke::licensing;

std::unique_ptr<qtlic::QtLicensingClient> qtlic::createQtLicensingClient(
        const qtlic::QtLicensingConfiguration &configuration,
        const std::string & accessToken,
        std::shared_ptr<const xdhttp::HTTPClient> httpClient,
        std::shared_ptr<const tenduke::jwt::JWTParser> jwtParser
)
{
    std::shared_ptr<xdlic::LicensingConfiguration> config(new xdlic::LicensingConfiguration(
        accessToken,
        configuration.getEndpoint().toString().toStdString()
    ));

    std::shared_ptr<xdlic::LicensingClient> licensingClient(new xdlic::DefaultLicensingClient(
        config,
        httpClient,
        jwtParser
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
        std::shared_ptr<QNetworkAccessManager> networkAccessManager,
        std::shared_ptr<const tenduke::jwt::JWTParser> jwtParser
)
{
    return qtlic::createQtLicensingClient(
        configuration,
        accessToken,
        qthttp::createQtHTTPClient(networkAccessManager),
        jwtParser
    );
}
