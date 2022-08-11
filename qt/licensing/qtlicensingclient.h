#ifndef QTLICENSINGCLIENT_H
#define QTLICENSINGCLIENT_H

#include "./qtlicensecheckoutrequest.h"
#include "licensing/licensingclient.h"
#include "licensing/licensingconfiguration.h"
#include "qt/oidc/qtoidcstate.h"

#include <memory>
#include <QObject>

namespace tenduke { namespace qt { namespace licensing {


/** QT-binding of tenduke::licensing::LicensingClient.
 *
 */
class QtLicensingClient : public QObject
{
    Q_OBJECT

public:
    /** Constructs new client.
     *
     *  @param configuration licensing configuration. Also used to refresh OAuth accesstoken.
     *         This should be shared config with the LicensingClient-service.
     *  @param licenses the actual service
     *  @param hardwareId -
     */
    QtLicensingClient(
            std::shared_ptr<tenduke::licensing::LicensingConfiguration> configuration,
            std::shared_ptr<tenduke::licensing::LicensingClient> licenses,
            const std::string &hardwareId
    )
        : configuration(configuration), licenses(licenses), hardwareId(hardwareId)
    {}

    /** Checks out single licensed item.
     *
     *  @param licensedItemName -
     *  @return checkout request (see doc for usage)
     */
    std::unique_ptr<QtLicenseCheckoutRequest> checkout(const QString licensedItemName);

    /** Checks out multiple licensed items.
     *
     *  @param licensedItemName -
     *  @return checkout request (see doc for usage)
     */
    std::unique_ptr<QtLicenseCheckoutRequest> checkout(const QStringList licensedItemNames);

public Q_SLOTS:
    void oauthStateRefreshed(tenduke::qt::oidc::QtOIDCState state);

private:
    const std::shared_ptr<tenduke::licensing::LicensingConfiguration> configuration;
    const std::shared_ptr<tenduke::licensing::LicensingClient> licenses;
    const std::string hardwareId;
};


}}}

#endif // QTLICENSINGCLIENT_H
