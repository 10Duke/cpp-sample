#ifndef TENDUKE_QT_OIDC_QTAUTODISCOVERYREQUEST_H
#define TENDUKE_QT_OIDC_QTAUTODISCOVERYREQUEST_H

#include <oidc/autodiscovery.h>

#include <QObject>
#include <QUrl>

namespace tenduke { namespace qt { namespace oidc {

class QtAutoDiscoveryRequest : public QObject
{
    Q_OBJECT
public:
    explicit QtAutoDiscoveryRequest(
            const std::shared_ptr<const tenduke::oauth::oidc::AutoDiscovery> autoDiscovery,
            const QUrl discoveryDocumentUrl
    );
    virtual ~QtAutoDiscoveryRequest() {}

    void execute();

Q_SIGNALS:
    void finished(tenduke::oauth::oidc::AutoDiscoveryResult result);
    void failed(std::exception_ptr error);

private:
    const QUrl discoveryDocumentUrl;
    const std::shared_ptr<const tenduke::oauth::oidc::AutoDiscovery> oidcConfiguration;
};

}}}

#endif // TENDUKE_QT_OIDC_QTAUTODISCOVERYREQUEST_H
