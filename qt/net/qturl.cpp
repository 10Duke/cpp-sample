#include "./qturl.h"

#include <QUrlQuery>

namespace net = tenduke::net;
namespace qtnet = tenduke::qt::net;

std::multimap<std::string, std::string> qtnet::QtURL::getQueryParameters() const
{
    QUrlQuery qry = QUrlQuery(url);
    QList<QPair<QString, QString>> queryItems = qry.queryItems(QUrl::FullyDecoded);

    std::multimap<std::string, std::string> queryParameters;
    for (int i = 0; i < queryItems.size(); i++) {
        QPair<QString, QString> item = queryItems.at(i);
        queryParameters.emplace(
            item.first.toStdString(),
            item.second.toStdString()
        );
    }

    return queryParameters;
}
