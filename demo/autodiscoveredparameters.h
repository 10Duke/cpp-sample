#ifndef AUTODISCOVEREDPARAMETERS_H
#define AUTODISCOVEREDPARAMETERS_H

#include "./basewindow.h"
#include "oidc/autodiscovery.h"
#include "qt/licensing/qtlicensingconfiguration.h"
#include "qt/oidc/qtautodiscoveryrequest.h"
#include "qt/oidc/qtoidcconfig.h"

#include <memory>


#include <QMainWindow>

namespace tenduke { namespace qt { namespace demo {

namespace Ui {
class AutoDiscoveredParameters;
}

class AutoDiscoveredParameters : public BaseWindow
{
    Q_OBJECT

public:
    explicit AutoDiscoveredParameters(
            const std::shared_ptr<const tenduke::oauth::oidc::AutoDiscovery> autodiscovery,
            QWidget *parent = nullptr
    );
    ~AutoDiscoveredParameters();

Q_SIGNALS:
    void parametersCollected(
            tenduke::qt::oidc::QtOIDCConfig oidcParameters,
            tenduke::qt::licensing::QtLicensingConfiguration licensingConfiguration
    );


private Q_SLOTS:
    void discover();
    void discoveryRequestFailed(std::exception_ptr error);
    void discoveryRequestFinished(tenduke::oauth::oidc::AutoDiscoveryResult result);
    void go();

private:
    const std::unique_ptr<Ui::AutoDiscoveredParameters> ui;
    const std::shared_ptr<const tenduke::oauth::oidc::AutoDiscovery> autoDiscovery;

    std::unique_ptr<tenduke::qt::oidc::QtAutoDiscoveryRequest> request;
    std::shared_ptr<const tenduke::oauth::OAuthBackendConfiguration> discoveredOAuthConfiguration;
    std::shared_ptr<const tenduke::oauth::oidc::OIDCConfiguration> discoveredOIDCConfiguration;
};

}}}

#endif // AUTODISCOVEREDPARAMETERS_H
