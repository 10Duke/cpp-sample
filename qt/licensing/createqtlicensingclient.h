#ifndef TENDUKE_QT_LICENSING_CREATEQTLICENSINGCLIENT_H
#define TENDUKE_QT_LICENSING_CREATEQTLICENSINGCLIENT_H

#include "./qtlicensingclient.h"
#include "./qtlicensingconfiguration.h"
#include "http/httpclient.h"
#include "json/jsonparser.h"

#include <QNetworkAccessManager>

namespace tenduke { namespace qt { namespace licensing {


/** Factory method for creating QtLicensingClient.
 *
 *  @param configuration -
 *  @param accessToken -
 *  @param httpClient -
 *  @param jsonParser -
 *  @return -
 */
std::unique_ptr<tenduke::qt::licensing::QtLicensingClient> createQtLicensingClient(
    const tenduke::qt::licensing::QtLicensingConfiguration &configuration,
    const std::string &accessToken,
    std::shared_ptr<tenduke::http::HTTPClient> httpClient,
    std::shared_ptr<tenduke::json::JSONParser> jsonParser
);

/** Factory method for creating QtLicensingClient.
 *
 *  @param configuration -
 *  @param accessToken -
 *  @param httpClient -
 *  @return -
 */
std::unique_ptr<tenduke::qt::licensing::QtLicensingClient> createQtLicensingClient(
    const tenduke::qt::licensing::QtLicensingConfiguration &configuration,
    const std::string &accessToken,
    std::shared_ptr<tenduke::http::HTTPClient> httpClient
);

/** Factory method for creating QtLicensingClient.
 *
 *  @param configuration -
 *  @param accessToken -
 *  @param networkAccessManager -
 *  @return -
 */
std::unique_ptr<tenduke::qt::licensing::QtLicensingClient> createQtLicensingClient(
    const tenduke::qt::licensing::QtLicensingConfiguration &configuration,
    const std::string &accessToken,
    std::shared_ptr<QNetworkAccessManager> networkAccessManager
);


}}}

#endif // TENDUKE_QT_LICENSING_CREATEQTLICENSINGCLIENT_H
