#include "./qtoidcclient.h"

#include "oidc/oidcexception.h"

#include <QByteArray>
#include <QString>
#include <QJsonDocument>
#include <QVariant>
#include <QtConcurrent/QtConcurrent>

#include "qt/utl/exceptiontostringlist.h"

namespace qtoidc = tenduke::qt::oidc;
namespace xdoauth = tenduke::oauth;
namespace xdoidc = tenduke::oauth::oidc;


qtoidc::QtOIDCClient::QtOIDCClient(const std::shared_ptr<xdoidc::OIDCClient> oidc)
    : oidc(oidc), state(nullptr), request(nullptr)
{}


qtoidc::QtOIDCClient::~QtOIDCClient()
{
}


void qtoidc::QtOIDCClient::authenticate(QString scopes)
{
    this->request = oidc->authenticate(scopes.toStdString());

    Q_EMIT openBrowser(QString::fromStdString(this->request->buildAuthenticationRequestUrl()));
}


void qtoidc::QtOIDCClient::callbackUrlNavigated(QUrl url)
{
    const std::string callbackUrl = url.toString().toStdString();

    try {
        this->state = this->request->handleCallback(callbackUrl);

        Q_EMIT authenticated(qtoidc::QtOIDCState(
            QString::fromStdString(state->getAccessToken()),
            state->getExpiresAt(),
            QString::fromStdString(state->getIdToken())
        ));
    }
    catch (const std::exception &e)  {
        Q_EMIT authenticationFailure(std::current_exception());
    }
}

bool qtoidc::QtOIDCClient::isAuthenticated() const
{
    return (state != nullptr);
}


void qtoidc::QtOIDCClient::refresh()
{

    try {
        if (state == nullptr) {
            throw xdoauth::OAuthException(
                        xdoauth::OAuthException::REFRESH_REQUEST,
                        "not_yet_authenticated",
                        "Not yet authenticated: Cannot refresh"
            );
        }

        state->refresh();

        Q_EMIT authenticated(qtoidc::QtOIDCState(
            QString::fromStdString(state->getAccessToken()),
            state->getExpiresAt(),
            QString::fromStdString(state->getIdToken())
        ));
    }
    catch (const std::exception &e) {
        Q_EMIT authenticationFailure(std::current_exception());
    }
}
