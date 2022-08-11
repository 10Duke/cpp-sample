#ifndef TENDUKE_QT_NET_QTURLPARSER_H
#define TENDUKE_QT_NET_QTURLPARSER_H

#include "net/urlparser.h"

namespace tenduke { namespace qt { namespace net {


/** QT implementation of tenduke::net::URLParser.
 *
 */
class QtURLParser : public tenduke::net::URLParser
{
    // URLParser interface
public:
    std::unique_ptr<tenduke::net::URL> from(const std::string &urlToParse) const override;
};


}}}

#endif // TENDUKE_QT_NET_QTURLPARSER_H
