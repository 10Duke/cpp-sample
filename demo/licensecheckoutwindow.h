#ifndef LICENSECHECKOUTWINDOW_H
#define LICENSECHECKOUTWINDOW_H

#include "./basewindow.h"
#include "qt/licensing/qtlicensingclient.h"
#include "qt/oauth/qtoauthstate.h"

#include <exception>
#include <memory>

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QStringList>
#include <QTableWidget>
#include <QUrl>


namespace tenduke { namespace qt { namespace demo {

namespace Ui {
class LicenseCheckoutWindow;
}

class LicenseCheckoutWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit LicenseCheckoutWindow(
            const std::shared_ptr<tenduke::qt::licensing::QtLicensingClient> license,
            QWidget *parent = nullptr
    );
    virtual ~LicenseCheckoutWindow();

protected Q_SLOTS:
    virtual void checkoutRequestFinished(tenduke::licensing::LicenseCheckoutResponse response);
    virtual void checkoutRequestFailed(std::exception_ptr error);

protected:
    virtual void fillTable(tenduke::licensing::LicenseCheckoutResponse &response);

private:
    void checkoutLicense();

    const std::unique_ptr<Ui::LicenseCheckoutWindow> ui;
    const std::shared_ptr<tenduke::qt::licensing::QtLicensingClient> license;
    std::unique_ptr<tenduke::qt::licensing::QtLicenseCheckoutRequest> request;
};

}}}

#endif // LICENSECHECKOUTWINDOW_H
