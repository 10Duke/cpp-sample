#ifndef QTOIDCCONFIG_H
#define QTOIDCCONFIG_H

#include "qt/oauth/qtoauthconfig.h"

#include <QString>
#include <QUrl>

namespace tenduke { namespace qt { namespace oidc {


/** OIDC-config.
 *
 */
class QtOIDCConfig : public tenduke::qt::oauth::QtOAuthConfig
{
public:
    QtOIDCConfig(
            const QUrl authorizationEndpoint,
            const QUrl tokenEndpoint,
            const QUrl userinfoEndpoint,
            const QString redirectUri,
            const QString clientIdentifier,
            const QString clientSecret,
            const QString issuer,
            const QString verificationKey
    ) : QtOAuthConfig(authorizationEndpoint, tokenEndpoint, userinfoEndpoint, redirectUri, clientIdentifier, clientSecret),
        issuer(issuer), verificationKey(verificationKey)
    {}

    const QString issuer;
    /** The verification key. Currently supported is RSA-key in PEM-format. If empty, ID token is not verified. */
    const QString verificationKey;
};


}}}

#endif // QTOIDCCONFIG_H
