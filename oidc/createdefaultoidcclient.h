#ifndef TENDUKE_OAUTH_OIDC_CREATEDEFAULTOIDCCLIENT_H
#define TENDUKE_OAUTH_OIDC_CREATEDEFAULTOIDCCLIENT_H

#include "./oidcclient.h"
#include "./oidcconfiguration.h"
#include "http/httpclient.h"
#include "json/jsonparser.h"
#include "time/clock.h"

#include <memory>


namespace tenduke { namespace oauth { namespace oidc {


std::unique_ptr<tenduke::oauth::oidc::OIDCClient> createDefaultOIDCClient(
        const std::shared_ptr<const tenduke::oauth::OAuthConfiguration> oauthConfiguration,
        const std::shared_ptr<const tenduke::oauth::oidc::OIDCConfiguration> oidcConfiguration,
        const std::shared_ptr<tenduke::http::HTTPClient> httpClient,
        const std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
        const std::shared_ptr<tenduke::utl::random::RandomBytes> randomGenerator,
        const std::shared_ptr<tenduke::time::Clock> clock
);


}}}


#endif // TENDUKE_OAUTH_OIDC_CREATEDEFAULTOIDCCLIENT_H
