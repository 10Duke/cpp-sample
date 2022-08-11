#include "./oidcclient.h"

#include "oauth/oauthrequest.h"

namespace oidc = tenduke::oauth::oidc;


static bool hasOpenIdScope(const std::string &scopes)
{
    // Not the most efficient implementation but will do for now:

    // "openid"
    if (scopes.size() == 6 && scopes == "openid") {
      return true;
    }

    // "openid abc"
    if (scopes.find("openid ") == 0) {
        return true;
    }

    // "abc openid def"
    // "abc openid "
    // " openid def"
    // " openid "
    if (scopes.find(" openid ") != std::string::npos) {
        return true;
    }

    // "abc openid"
    if (scopes.find(" openid") == scopes.size() - 6) {
        return true;
    }

    return false;
}


std::unique_ptr<const oidc::OIDCRequest> oidc::OIDCClient::authenticate(const std::string &scopes)
{
    std::string effectiveScopes = scopes;

    if (! hasOpenIdScope(effectiveScopes) ) {
        effectiveScopes = "openid";
        if (! scopes.empty()) {
            effectiveScopes += ' ';
            effectiveScopes += scopes;
        }
    }

    std::string nonce = generateNonce();
    std::map<std::string, std::string> additionalParameters;
    additionalParameters["nonce"] = nonce;

    return std::unique_ptr<const oidc::OIDCRequest>(new oidc::OIDCRequest(
        oauthClient->authorize(effectiveScopes, additionalParameters),
        oidcConfiguration,
        nonce,
        parseJWT
    ));
}


std::string oidc::OIDCClient::generateNonce()
{
    return randomUrlSafeString->generate(nonceLength);
}

