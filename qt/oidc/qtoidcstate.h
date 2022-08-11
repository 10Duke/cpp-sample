#ifndef QTOIDCSTATE_H
#define QTOIDCSTATE_H

#include "qt/oauth/qtoauthstate.h"

#include <QString>

namespace tenduke { namespace qt { namespace oidc {


/** QT-specific OIDC-state.
 *
 */
class QtOIDCState : public tenduke::qt::oauth::QtOAuthState
{
public:
    QtOIDCState(
            const QString accessToken,
            int64_t expiresAt,
            const QString idToken
    ) : tenduke::qt::oauth::QtOAuthState(accessToken, expiresAt), idToken(idToken)
    {}

    const QString getIdToken() const {return idToken;}

private:
    QString idToken;
};


}}}

#endif // QTOIDCSTATE_H
