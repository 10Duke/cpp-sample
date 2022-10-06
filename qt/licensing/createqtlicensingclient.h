#ifndef TENDUKE_QT_LICENSING_CREATEQTLICENSINGCLIENT_H
#define TENDUKE_QT_LICENSING_CREATEQTLICENSINGCLIENT_H

#include "./qtlicensingclient.h"
#include "./qtlicensingconfiguration.h"
#include "http/httpclient.h"
#include "jwt/jwtparser.h"

#include <QNetworkAccessManager>

namespace tenduke { namespace qt { namespace licensing {


/** Factory method for creating QtLicensingClient.
 *
 *  @param configuration -
 *  @param accessToken -
 *  @param httpClient -
 *  @param jwtParser -
 *  @return -
 */
std::unique_ptr<tenduke::qt::licensing::QtLicensingClient> createQtLicensingClient(
    const tenduke::qt::licensing::QtLicensingConfiguration &configuration,
    const std::string &accessToken,
    std::shared_ptr<const tenduke::http::HTTPClient> httpClient,
    std::shared_ptr<const tenduke::jwt::JWTParser> jwtParser
);

/** Factory method for creating QtLicensingClient.
 *
 *  @param configuration -
 *  @param accessToken -
 *  @param networkAccessManager -
 *  @param jwtParser -
 *  @return -
 */
std::unique_ptr<tenduke::qt::licensing::QtLicensingClient> createQtLicensingClient(
    const tenduke::qt::licensing::QtLicensingConfiguration &configuration,
    const std::string &accessToken,
    std::shared_ptr<QNetworkAccessManager> networkAccessManager,
    std::shared_ptr<const tenduke::jwt::JWTParser> jwtParser
);


}}}

#endif // TENDUKE_QT_LICENSING_CREATEQTLICENSINGCLIENT_H
