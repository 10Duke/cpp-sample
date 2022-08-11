#ifndef QTOAUTHSTATE_H
#define QTOAUTHSTATE_H

#include <cstdint>

#include <QString>

namespace tenduke { namespace qt { namespace oauth {


/** Simple value-class to relay OAuthState.
 *  Not same as tenduke::oauth::OAuthState, which is also a service (allows refreshing the state).
 *
 */
class QtOAuthState
{
public:
    QtOAuthState(
            const QString accessToken,
            int64_t expiresAt
    ) : accessToken(accessToken), expiresAt(expiresAt)
    {}

    const QString getAccessToken() const {return accessToken;}
    int64_t getExpiresAt() const {return expiresAt;}

private:
    QString accessToken;
    int64_t expiresAt;
};


}}}

#endif // QTOAUTHSTATE_H
