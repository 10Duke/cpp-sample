#include "./qtlicensingclient.h"

#include "licensing/licensecheckoutparametersbuilder.h"

// TODO: Remove following line
#include <iostream>


namespace qtlic = tenduke::qt::licensing;
namespace qtoidc = tenduke::qt::oidc;
namespace xdlic = tenduke::licensing;

std::unique_ptr<qtlic::QtLicenseCheckoutRequest> qtlic::QtLicensingClient::checkout(const QString licensedItemName)
{
    QStringList names(licensedItemName);

    return checkout(names);
}

std::unique_ptr<qtlic::QtLicenseCheckoutRequest> qtlic::QtLicensingClient::checkout(const QStringList licensedItemNames)
{
    xdlic::LicenseCheckoutParametersBuilder request;

    request.hardwareId(hardwareId);

    for (auto licensedItemName : licensedItemNames) {
        request.item(licensedItemName.toStdString());
    }

    return std::unique_ptr<qtlic::QtLicenseCheckoutRequest>(new qtlic::QtLicenseCheckoutRequest(
        licenses->checkout(request.build()
    )));
}


void qtlic::QtLicensingClient::oauthStateRefreshed(qtoidc::QtOIDCState state)
{
    std::cerr << "INFO QtLicensingClient::oauthStateRefreshed()" << std::endl;

    configuration->setAccessToken(state.getAccessToken().toStdString());
}
