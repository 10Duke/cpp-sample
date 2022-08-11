#ifndef QTOIDCCLIENT_H
#define QTOIDCCLIENT_H

#include "./qtoidcstate.h"
#include "oidc/oidcclient.h"
#include "oidc/oidcrequest.h"
#include "oidc/oidcstate.h"

#include <exception>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QUrl>

#include <memory>

namespace tenduke { namespace qt { namespace oidc {


/** QT-binding for OpenIDConnect client.
 *
 *  The QtOIDCClient maintains the login state internally, so the instance should
 *  be kept alive for scope of the application. This also means that multiple OIDC-sessions
 *  need one instance per session.
 *
 *  Authentication starts by calling authenticate(), which emits signal openBrowser().
 *  The login UI must listen this signal and open / navigate browser to emitted URL.
 *
 *  Once the login flow is complete, the server will do HTTP redirect to the callback URL.
 *  The login UI must detect this navigation and then call callBackUrlNavigated(), which
 *  performs rest of the steps. These steps will eventually emit either authenticated()
 *  or authenticationFailure() signal.
 *
 *  Following exceptions (or their subclasses) can be signalled via authenticationFailure():
 *  - tenduke::net::NetworkingException (or subclasses) in case network communication failed for various reasons
 *  - tenduke::http::HTTP...
 *  - tenduke::oauth::OAuthException in case server reported OAuth-error condition
 *  - tenduke::oidc::OIDCException if something failed in the OIDC-process
 *
 */
class QtOIDCClient : public QObject
{
    Q_OBJECT

public:
    QtOIDCClient(const std::shared_ptr<tenduke::oauth::oidc::OIDCClient> oidc);
    virtual ~QtOIDCClient();

    virtual void authenticate(QString scopes);
    virtual bool isAuthenticated() const;

    /** Refreshes the OIDC state.
     *  Once complete, the authenticated()-signal will fire.
     *  If refresh fails, authenticationFailure()-signal will fire.
     *
     */
    virtual void refresh();

public Q_SLOTS:
    virtual void callbackUrlNavigated(QUrl url);

Q_SIGNALS:
    void openBrowser(QString url);
    void authenticated(tenduke::qt::oidc::QtOIDCState state);
    void authenticationFailure(std::exception_ptr error);

private:
    const std::shared_ptr<tenduke::oauth::oidc::OIDCClient> oidc;
    std::shared_ptr<tenduke::oauth::oidc::OIDCState> state;
    std::unique_ptr<const tenduke::oauth::oidc::OIDCRequest> request;
};


}}}

#endif // QTOIDCCLIENT_H
