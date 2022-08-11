#ifndef QTLICENSECHECKOUTREQUEST_H
#define QTLICENSECHECKOUTREQUEST_H

#include "licensing/licensecheckoutrequest.h"
#include "licensing/licensecheckoutresponse.h"

#include <exception>
#include <memory>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QStringList>

namespace tenduke { namespace qt { namespace licensing {


/** QT-binding for tenduke::licensing::LicenseCheckoutRequest.
 *
 *  Use this as follows:
 *         QtLicensingClient *licenses; // get this from somewhere
 *
 *         std::unique_ptr<QtLicenseCheckoutRequest> request = licenses->checkout(const QString licensedItemName);
 *         QObject::connect(
 *             request.get(), &tenduke::qt::licensing::QtLicenseCheckoutRequest::finished,
 *             this, &SomeClass::someSuccessMethod
 *         );
 *         QObject::connect(
 *             request.get(), &tenduke::qt::licensing::QtLicenseCheckoutRequest::failed,
 *             this, &SomeClass::someFailureMethod
 *         );
 *         request->execute();
 *
 */
class QtLicenseCheckoutRequest : public QObject
{
    Q_OBJECT

public:
    QtLicenseCheckoutRequest(std::unique_ptr<tenduke::licensing::LicenseCheckoutRequest> request);
    virtual ~QtLicenseCheckoutRequest();

    /** Executes the request.
     *
     *  Signals are used to respond.
     */
    void execute();

Q_SIGNALS:
    void finished(tenduke::licensing::LicenseCheckoutResponse response);
    void failed(std::exception_ptr error);

private:
    const std::unique_ptr<tenduke::licensing::LicenseCheckoutRequest> request;
};


}}}

#endif // QTLICENSECHECKOUTREQUEST_H
