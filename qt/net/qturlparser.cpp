#include "./qturlparser.h"
#include "./qturl.h"

#include <QString>

namespace xdnet = tenduke::net;
namespace qtnet = tenduke::qt::net;

std::unique_ptr<xdnet::URL> qtnet::QtURLParser::from(const std::string &urlToParse) const
{
    QUrl url(QString::fromStdString(urlToParse));
    return std::unique_ptr<xdnet::URL>(new qtnet::QtURL(url));
}
