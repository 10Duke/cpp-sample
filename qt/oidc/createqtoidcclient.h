#ifndef CREATEQTOUATH_H
#define CREATEQTOUATH_H

#include "./qtoidcclient.h"
#include "./qtoidcconfig.h"
#include "http/httpclient.h"
#include "json/jsonparser.h"
#include "oauth/oauthconfiguration.h"
#include "oidc/autodiscovery.h"
#include "oidc/oidcconfiguration.h"
#include "oidc/oidcclient.h"
#include "time/clock.h"

#include <memory>
#include <QNetworkAccessManager>


namespace tenduke { namespace qt { namespace oidc {

std::unique_ptr<const tenduke::oauth::oidc::AutoDiscovery> createAutoDiscovery(std::shared_ptr<const tenduke::http::HTTPClient> httpClient);

/** Factory method for creating QtOIDCClient.
 *
 *  @param oauthConfiguration -
 *  @param oidcConfiguration -
 *  @param httpClient -
 *  @return -
 */
std::unique_ptr<tenduke::qt::oidc::QtOIDCClient> createQtOIDCClient(
    std::shared_ptr<const tenduke::oauth::OAuthConfiguration> oauthConfiguration,
    std::shared_ptr<const tenduke::oauth::oidc::OIDCConfiguration> oidcConfiguration,
    std::shared_ptr<const tenduke::http::HTTPClient> httpClient
);

/** Factory method for creating QtOIDCClient.
 *
 *  @param oauthConfiguration -
 *  @param oidcConfiguration -
 *  @param httpClient -
 *  @param jsonParser -
 *  @param clock -
 *  @return -
 */
std::unique_ptr<tenduke::qt::oidc::QtOIDCClient> createQtOIDCClient(
    std::shared_ptr<const tenduke::oauth::OAuthConfiguration> oauthConfiguration,
    std::shared_ptr<const tenduke::oauth::oidc::OIDCConfiguration> oidcConfiguration,
    std::shared_ptr<const tenduke::http::HTTPClient> httpClient,
    std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
    std::shared_ptr<tenduke::time::Clock> clock
);

/** Factory method for creating QtOIDCClient.
 *
 *  @param oidcConfiguration -
 *  @param httpClient -
 *  @return -
 */
std::shared_ptr<tenduke::qt::oidc::QtOIDCClient> createOidcClient(
        std::shared_ptr<const tenduke::qt::oidc::QtOIDCConfig> oidcConfiguration,
        std::shared_ptr<const tenduke::http::HTTPClient> httpClient
);



}}}

#endif // CREATEQTOUATH_H
