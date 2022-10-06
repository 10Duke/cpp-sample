#include "autodiscoveredparameters.h"
#include "ui_autodiscoveredparameters.h"

#include "crypto/publickeytopem.h"

#include <QString>
#include <iostream>

namespace qtdemo = tenduke::qt::demo;
namespace qtlic = tenduke::qt::licensing;
namespace qtoidc = tenduke::qt::oidc;
namespace xdlibcrypto = tenduke::crypto::libcrypto;
namespace xdoauth = tenduke::oauth;
namespace xdoidc = tenduke::oauth::oidc;

qtdemo::AutoDiscoveredParameters::AutoDiscoveredParameters(
        const std::shared_ptr<const tenduke::oauth::oidc::AutoDiscovery> autodiscovery,
        QWidget *parent
) :
    BaseWindow(parent),
    ui(std::unique_ptr<Ui::AutoDiscoveredParameters>(new Ui::AutoDiscoveredParameters())),
    autoDiscovery(autodiscovery),
    request(nullptr),
    discoveredOAuthConfiguration(nullptr),
    discoveredOIDCConfiguration(nullptr)
{
    ui->setupUi(this);

    QObject::connect(
        ui->btnDiscover, &QPushButton::clicked,
        this, &qtdemo::AutoDiscoveredParameters::discover
    );
    QObject::connect(
        ui->btnGo, &QPushButton::clicked,
        this, &qtdemo::AutoDiscoveredParameters::go
    );
}

qtdemo::AutoDiscoveredParameters::~AutoDiscoveredParameters()
{
    // Intentionally empty
}

void qtdemo::AutoDiscoveredParameters::go()
{
    if (discoveredOAuthConfiguration == nullptr) {
        std::cerr << "Need config!" << std::endl;
        return;
    }

    // Convert the provided Public Key to PEM
    // We have working Public Key, which we serialize to PEM, which is then later converted back to Public Key
    // when the OIDC-client is constructed. The serialized key is safer to pass around.
    std::string verificationKeyAsPEM = xdlibcrypto::PublicKeyToPEM().from(*(discoveredOIDCConfiguration->verificationKey.get()));

    Q_EMIT parametersCollected(
        tenduke::qt::oidc::QtOIDCConfig (
            QUrl(QString::fromStdString(discoveredOAuthConfiguration->authorizationEndpointUrl)),
            QUrl(QString::fromStdString(discoveredOAuthConfiguration->tokenEndpointUrl)),
            QUrl(QString::fromStdString(discoveredOIDCConfiguration->userinfoEndpoint)),
            ui->redirectUri->text(),
            ui->clientIdentifier->text(),
            ui->clientSecret->text(),
            ui->issuer->text(),
            QString::fromStdString(verificationKeyAsPEM)
        ),
        qtlic::QtLicensingConfiguration(
            QUrl(ui->licensingEndpoint->text()),
            ui->hardwareId->text()
        )
    );
}


void qtdemo::AutoDiscoveredParameters::discover()
{
    ui->btnDiscover->setEnabled(false);
    discoveredOAuthConfiguration = nullptr;
    discoveredOIDCConfiguration = nullptr;

    QString discoveryDocumentUrl = ui->hostname->text();
    QString licensingUrl;

    if (! discoveryDocumentUrl.endsWith('/')) {
        discoveryDocumentUrl.append('/');
    }
    licensingUrl = QString(discoveryDocumentUrl).append("authz/");
    discoveryDocumentUrl.append(".well-known/openid-configuration");
    ui->licensingEndpoint->setText(licensingUrl);

    request = std::unique_ptr<qtoidc::QtAutoDiscoveryRequest>(new qtoidc::QtAutoDiscoveryRequest(
        autoDiscovery,
        QUrl(discoveryDocumentUrl)
    ));

    QObject::connect(
        request.get(), &qtoidc::QtAutoDiscoveryRequest::finished,
        this, &AutoDiscoveredParameters::discoveryRequestFinished
    );
    QObject::connect(
        request.get(), &qtoidc::QtAutoDiscoveryRequest::failed,
        this, &AutoDiscoveredParameters::discoveryRequestFailed
    );

    request->execute();

}


void qtdemo::AutoDiscoveredParameters::discoveryRequestFailed(std::exception_ptr error)
{
    // This demo should open some alert and show the error message there
    // but now just dumps the error to console.
    std::cerr << "The request failed:" << std::endl;
    try {
        std::rethrow_exception(error);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    ui->btnDiscover->setEnabled(true);
}


void qtdemo::AutoDiscoveredParameters::discoveryRequestFinished(xdoidc::AutoDiscoveryResult result)
{
    discoveredOAuthConfiguration = result.getOAuthConfiguration();
    // Note how we use the AutoDiscovery-service to get the OIDC-configuration.
    // The AutoDiscovery-service generates working PublicKey from the JWKS-document.
    // The PublicKey is converted to PEM in `go()`-method.
    discoveredOIDCConfiguration = autoDiscovery->getOIDCConfiguration(result);

    ui->authorizationEndpoint->setText(QString::fromStdString(discoveredOAuthConfiguration->authorizationEndpointUrl));
    ui->tokenEndpoint->setText(QString::fromStdString(discoveredOAuthConfiguration->tokenEndpointUrl));
    ui->userinfoEndpoint->setText(QString::fromStdString(discoveredOIDCConfiguration->userinfoEndpoint));
    ui->issuer->setText(QString::fromStdString(discoveredOIDCConfiguration->issuer));

    ui->btnDiscover->setEnabled(true);
}
