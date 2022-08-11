#ifndef TENDUKE_QT_HTTP_CREATEQTHTTPCLIENT_H
#define TENDUKE_QT_HTTP_CREATEQTHTTPCLIENT_H

#include "http/httpclient.h"
#include "time/clock.h"

#include <memory>
#include <QNetworkAccessManager>


namespace tenduke { namespace qt { namespace http {


/** Factory method for creating QtHTTPClient.
 *
 *  @param networkAccessManager -
 *  @return -
 */
std::unique_ptr<tenduke::http::HTTPClient> createQtHTTPClient(
    std::shared_ptr<QNetworkAccessManager> networkAccessManager
);

/** Factory method for creating QtHTTPClient.
 *
 *  @param networkAccessManager -
 *  @param clock -
 *  @return -
 */
std::unique_ptr<tenduke::http::HTTPClient> createQtHTTPClient(
    std::shared_ptr<QNetworkAccessManager> networkAccessManager,
    std::shared_ptr<tenduke::time::Clock> clock
);


}}}

#endif // TENDUKE_QT_HTTP_CREATEQTHTTPCLIENT_H
