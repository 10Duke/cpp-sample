#ifndef TENDUKE_QT_HTTP_QTHTTPCALL_H
#define TENDUKE_QT_HTTP_QTHTTPCALL_H

#include "http/httpcall.h"
#include "http/httprequest.h"
#include "time/clock.h"

#include <memory>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>


namespace tenduke { namespace qt { namespace http {


/** QT-implementation of HTTP-call.
 *
 *  Uses QNetworkAccessManager under-the-hood.
 */
class QtHTTPCall : public QObject, public tenduke::http::HTTPCall
{
    Q_OBJECT

public:
    QtHTTPCall(
        std::shared_ptr<const tenduke::http::HTTPRequest> request,
        std::shared_ptr<QNetworkAccessManager> networkAccessManager,
        std::shared_ptr<tenduke::time::Clock> clock
    );
    virtual ~QtHTTPCall();

    // tenduke::http::HTTPCall interface
public:
    virtual std::unique_ptr<tenduke::http::HTTPResponse> execute() override;

public Q_SLOTS:
    void networkReplyFinished();

protected:
    QNetworkRequest createNetworkRequest();
    std::unique_ptr<tenduke::http::HTTPResponse> createResponse();

private:
    const std::shared_ptr<const tenduke::http::HTTPRequest> request;
    const std::shared_ptr<QNetworkAccessManager> networkAccessManager;
    const std::shared_ptr<tenduke::time::Clock> clock;
    QNetworkReply * reply;
};


}}}


#endif // TENDUKE_QT_HTTP_QTHTTPCALL_H
