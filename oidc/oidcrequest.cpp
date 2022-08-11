#include "./oidcrequest.h"
#include "./oidcexception.h"
#include "jwt/jwtparsingexception.h"
#include "oauth/oauthexception.h"

namespace oidc = tenduke::oauth::oidc;
namespace oauth = tenduke::oauth;
namespace jwt = tenduke::jwt;


std::string oidc::OIDCRequest::buildAuthenticationRequestUrl() const
{
    return oauthRequest->buildAuthorizationRequestUrl();
}


std::unique_ptr<oidc::OIDCState> oidc::OIDCRequest::handleCallback(const std::string &callbackUrl) const
{
    std::unique_ptr<oauth::OAuthState> oauthState;

    oauthState = oauthRequest->handleCallback(callbackUrl);

    const std::string * idToken = oauthState->getAdditionalProperty("id_token");
    if (idToken == nullptr) {
        throw oidc::OIDCException (
            oauth::OAuthException::Phase::TOKEN_REQUEST,
            "no_id_token",
            "No id token in response!"
        );
    }

    validateIdToken(*idToken);

    return std::unique_ptr<oidc::OIDCState>(new oidc::OIDCState(std::move(oauthState), *idToken));
}


void oidc::OIDCRequest::validateIdToken(const std::string &idToken)
const
{
    try {
        jwt::JWT jwt = parseJWT->from(idToken);

        const std::string * claim = jwt.claim("iss");
        if (claim == nullptr) {
            throw oidc::OIDCException (
                oauth::OAuthException::Phase::TOKEN_REQUEST,
                "invalid_id_token",
                "Missing \"iss\"-claim"
            );
        }
        if (*claim != oidcConfiguration->issuer) {
            throw oidc::OIDCException (
                oauth::OAuthException::Phase::TOKEN_REQUEST,
                "invalid_issuer",
                "Invalid issuer: " + *claim
            );
        }

        // TODO: Validate "aud" and "azp"

        claim = jwt.claim("nonce");
        if (claim == nullptr) {
            throw oidc::OIDCException (
                oauth::OAuthException::Phase::TOKEN_REQUEST,
                "invalid_id_token",
                "Missing \"nonce\""
            );
        }
        if (*claim != nonce) {
            throw oidc::OIDCException (
                oauth::OAuthException::Phase::TOKEN_REQUEST,
                "invalid_nonce",
                "Nonce does not match"
            );
        }
    }
    catch (jwt::JWTParsingException &e) {
        std::throw_with_nested(oidc::OIDCException(
            oauth::OAuthException::Phase::TOKEN_REQUEST,
            "id_token_does_not_parse",
            "ID-token does not parse"
        ));
    }
}
