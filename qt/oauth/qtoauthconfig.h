#ifndef QTOAUTHCONFIG_H
#define QTOAUTHCONFIG_H

#include <QString>
#include <QUrl>

namespace tenduke { namespace qt { namespace oauth {


/** QT-specific OAuthConfig.
 *
 */
class QtOAuthConfig
{
public:
    QtOAuthConfig(
            const QUrl authorizationEndpoint,
            const QUrl tokenEndpoint,
            const QUrl userinfoEndpoint,
            const QString redirectUri,
            const QString clientIdentifier,
            const QString clientSecret
    ) : authorizationEndpoint(authorizationEndpoint),
        tokenEndpoint(tokenEndpoint),
        userinfoEndpoint(userinfoEndpoint),
        redirectUri(redirectUri),
        clientIdentifier(clientIdentifier),
        clientSecret(clientSecret)
    {}

    QUrl getAuthorizationEndpoint() const {return authorizationEndpoint;}
    QUrl getTokenEndpoint() const {return tokenEndpoint;}
    QUrl getUserinfoEndpoint() const {return userinfoEndpoint;}
    QString getRedirectUri() const {return redirectUri;}
    QString getClientIdentifier() const {return clientIdentifier;}
    QString getClientSecret() const {return clientSecret;}

private:
    const QUrl authorizationEndpoint;
    const QUrl tokenEndpoint;
    const QUrl userinfoEndpoint;
    const QString redirectUri;
    const QString clientIdentifier;
    const QString clientSecret;
};


}}}

#endif // QTOAUTHCONFIG_H
