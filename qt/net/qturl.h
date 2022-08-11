#ifndef TENDUKE_QT_NET_QTURL_H
#define TENDUKE_QT_NET_QTURL_H

#include "net/url.h"

#include <QUrl>

namespace tenduke { namespace qt { namespace net {


/** QT implementation of tenduke::net::URL.
 *
 */
class QtURL : public tenduke::net::URL
{
public:
    QtURL(QUrl url) : url(url) {}


    // tenduke::net::URL interface
public:
    std::multimap<std::string, std::string> getQueryParameters() const override;

private:
    QUrl url;
};


}}}

#endif // TENDUKE_QT_NET_QTURL_H
