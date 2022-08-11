#ifndef TENDUKE_QT_NET_QTURLENCODER_H
#define TENDUKE_QT_NET_QTURLENCODER_H

#include "net/urlencoder.h"

namespace tenduke { namespace qt { namespace net {


/** QTimplementation of tenduke::net::URLEnccoder.
 *
 */
class QtURLEncoder : public tenduke::net::URLEncoder
{
public:
    virtual ~QtURLEncoder() {}

    // URLEncoder interface
public:
    std::string encode(const std::string &stringToEncode) const override;
};


}}}

#endif // TENDUKE_QT_NET_QTURLENCODER_H
