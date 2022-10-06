#include "qtautodiscoveryrequest.h"

namespace qtoidc = tenduke::qt::oidc;
namespace xdoidc = tenduke::oauth::oidc;

qtoidc::QtAutoDiscoveryRequest::QtAutoDiscoveryRequest(
        const std::shared_ptr<const tenduke::oauth::oidc::AutoDiscovery> autoDiscovery,
        const QUrl discoveryDocumentUrl
)
    : discoveryDocumentUrl(discoveryDocumentUrl), oidcConfiguration(autoDiscovery)
{
}


void qtoidc::QtAutoDiscoveryRequest::execute()
{
    try {
        xdoidc::AutoDiscoveryResult result = oidcConfiguration->discover(discoveryDocumentUrl.toString().toStdString());
        Q_EMIT finished(result);
    }
    catch (const std::exception &e) {
        Q_EMIT failed(std::current_exception());
    }
}
