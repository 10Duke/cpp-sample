#ifndef OIDCSTATE_H
#define OIDCSTATE_H

#include "oauth/oauthstate.h"

#include <cstdint>
#include <memory>

namespace tenduke { namespace oauth { namespace oidc {


/** OIDC-state.
 *
 */
class OIDCState : public tenduke::oauth::OAuthState
{
public:
    OIDCState(
            std::unique_ptr<tenduke::oauth::OAuthState> oauthState,
            const std::string idToken
    )
        : oauthState(std::move(oauthState)), idToken(idToken)
    {}
    virtual ~OIDCState() = default;

    // tenduke::oauth::OAuthState interface
    virtual const std::string & getAccessToken() const override {return oauthState->getAccessToken();}
    virtual const std::string & getRefreshToken() const override {return oauthState->getRefreshToken();}
    virtual int64_t getExpiresAt() const override {return oauthState->getExpiresAt();}
    virtual const std::string * getAdditionalProperty(const std::string &name) const override {return oauthState->getAdditionalProperty(name);}
    virtual bool refresh() override {return oauthState->refresh();}

    // OIDC methods
    const std::string getIdToken() const {return idToken;}

private:
    const std::unique_ptr<tenduke::oauth::OAuthState> oauthState;
    const std::string idToken;
};


}}}

#endif // OIDCSTATE_H
