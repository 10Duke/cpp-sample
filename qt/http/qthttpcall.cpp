#include "./qthttpcall.h"

#include "net/networkingexception.h"
#include "net/requesttimedout.h"
#include "net/unknownhostexception.h"
#include "utl/fixedsizebinarydata.h"

#include <cstring>
#include <QByteArray>
#include <QThread>
#include <QUrl>

#include <QEventLoop>
#include <QTimer>

namespace net = tenduke::net;
namespace qthttp = tenduke::qt::http;
namespace utl = tenduke::utl;
namespace xdhttp = tenduke::http;

// Hard timeout for sleeping in the eventloop.
const std::int64_t HARD_TIMEOUT_MS = 600L * 1000L;


qthttp::QtHTTPCall::QtHTTPCall(
    std::shared_ptr<const tenduke::http::HTTPRequest> request,
    std::shared_ptr<QNetworkAccessManager> networkAccessManager,
    std::shared_ptr<tenduke::time::Clock> clock
) : request(request), networkAccessManager(networkAccessManager), clock(clock),
    reply(nullptr)
{
}


qthttp::QtHTTPCall::~QtHTTPCall()
{
    if (reply != nullptr) {
        reply->deleteLater();
    }
}


std::unique_ptr<xdhttp::HTTPResponse> qthttp::QtHTTPCall::execute()
{
    QEventLoop eventLoop;
    QTimer timer;

    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    connect(networkAccessManager.get(), SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // Build timeout for sleeping. If no timeout specified, use hard timeout anyway.
    std::int64_t timeoutMs = request->getTimeoutMs();
    if (timeoutMs <= 0L) {
        timeoutMs = HARD_TIMEOUT_MS;
    }

    reply = networkAccessManager->sendCustomRequest(
                createNetworkRequest(),
                QByteArray::fromStdString(request->getMethod()),
                QByteArray::fromStdString(request->getMessageBody())
    );

    timer.start(timeoutMs);
    eventLoop.exec();

    if(timer.isActive()){
        // Network access finished
        timer.stop();
    } else {
        // Network access timed out
    }

    if (reply->isFinished()) {
        enum QNetworkReply::NetworkError status = reply->error();

        // Handle specific cases:
        switch (status)
        {
            case QNetworkReply::NoError:
            case QNetworkReply::ProtocolInvalidOperationError:      // Maps from HTTP 400
                return createResponse();
            case QNetworkReply::ConnectionRefusedError:
                throw net::NetworkingException("Connection refused");
            case QNetworkReply::HostNotFoundError:
                throw net::UnknownHostException("Unknown host");
            case QNetworkReply::OperationCanceledError:             // Seems to be returned if setTransferTimeout exceeded
            case QNetworkReply::TimeoutError:
                throw net::RequestTimedOut("Request timed out");
            case QNetworkReply::TemporaryNetworkFailureError:
                throw net::NetworkingException("Temporary network failure");
            default:
                break;
        }

        // Handle network layer errors:
        if (status < 100) {
            throw net::NetworkingException("Networking failure (QNetworkReply::NetworkError " + std::to_string(status) + ")");
        }
        // Proxy errors
        if (status < 200) {
            throw net::NetworkingException("Networking proxy error (QNetworkReply::NetworkError " + std::to_string(status) + ")");
        }
        // HTTP-4xx???
        if (status < 300) {
            return createResponse();
        }
        // Protocol errors
        if (status < 400) {
            throw net::NetworkingException("Networking protocol error (QNetworkReply::NetworkError " + std::to_string(status) + ")");
        }
        // HTTP-5xx???
        if (status < 500) {
            return createResponse();
        }
        throw net::NetworkingException("Unknown networking error (QNetworkReply::NetworkError " + std::to_string(status) + ")");
    }

    // Reply did not finish for the duration of the request timeout
    reply->abort();
    throw net::RequestTimedOut("Request timed out after " + std::to_string(timeoutMs) + " milliseconds");
}

QNetworkRequest qthttp::QtHTTPCall::createNetworkRequest()
{
    QNetworkRequest networkRequest;

    networkRequest.setUrl(QUrl(QString::fromStdString(request->getUrl())));

    for (auto const & header : request->getHeaders()) {
        networkRequest.setRawHeader(
            QByteArray::fromStdString(header.first),
            QByteArray::fromStdString(header.second)
        );
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    if (request->getTimeoutMs() != 0) {
        networkRequest.setTransferTimeout(request->getTimeoutMs());
    }
#endif

    return networkRequest;
}

std::unique_ptr<xdhttp::HTTPResponse> qthttp::QtHTTPCall::createResponse()
{
    std::multimap<std::string, std::string> headers;

    for (auto header : reply->rawHeaderPairs()) {
        headers.emplace(
                    header.first.toStdString(),
                    header.second.toStdString()
        );
    }

    QByteArray responseBody = reply->readAll();
    std::unique_ptr<utl::FixedSizeBinaryData> data(new utl::FixedSizeBinaryData(responseBody.size()));
    std::memcpy(data.get()->getData(), responseBody.data(), responseBody.size());

    return std::unique_ptr<xdhttp::HTTPResponse>(new xdhttp::HTTPResponse(
        reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toInt(),
        headers,
        std::move(data)
    ));
}

void qthttp::QtHTTPCall::networkReplyFinished()
{
    // TODO: Do we need this?
}




