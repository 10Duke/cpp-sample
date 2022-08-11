#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./licensecheckoutwindow.h"
#include "./logindialog.h"
#include "./demoparametersdialog.h"
#include "./userinfowindow.h"

#include "http/httpclient.h"
#include "qt/licensing/qtlicensingconfiguration.h"
#include "qt/oidc/qtoidcconfig.h"
#include "qt/oidc/qtoidcstate.h"
#include "qt/webengine/customschemehandler.h"

#include <memory>
#include <QMainWindow>
#include <QNetworkAccessManager>

namespace tenduke { namespace qt { namespace demo {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(
            const std::shared_ptr<tenduke::qt::webengine::CustomSchemeHandler> schemeHandler,
            const std::shared_ptr<QNetworkAccessManager> networkAccessManager,
            QWidget *parent = nullptr
    );
    ~MainWindow();

    void login();

public Q_SLOTS:
    void accessTokenRefreshFailed(std::exception_ptr error);
    void loginFailed(std::exception_ptr error);
    void loginSuccessful(tenduke::qt::oidc::QtOIDCState state);
    void openLicenseWindow();
    void openParametersDialog();
    void openUserInfo();
    void parametersReady(
            tenduke::qt::oidc::QtOIDCConfig oidcParameters,
            tenduke::qt::licensing::QtLicensingConfiguration licensingParameters
    );
    void refreshAccessToken();

private Q_SLOTS:
    void closeAndRemoveLicenseWindow();
    void closeAndRemoveLoginDialog();
    void closeAndRemoveParametersDialog();
    void closeAndRemoveUserInfoDialog();

private:
    const std::unique_ptr<Ui::MainWindow> ui;

    std::shared_ptr<tenduke::qt::licensing::QtLicensingConfiguration> licensingParameters;
    std::shared_ptr<tenduke::qt::oidc::QtOIDCConfig> oidcParameters;
    std::shared_ptr<tenduke::qt::oidc::QtOIDCState> oidcState;
    const std::shared_ptr<tenduke::qt::webengine::CustomSchemeHandler> schemeHandler;
    const std::shared_ptr<QNetworkAccessManager> networkAccessManager;
    const std::shared_ptr<tenduke::http::HTTPClient> httpClient;
    std::shared_ptr<tenduke::qt::oidc::QtOIDCClient> oidc;
    std::shared_ptr<tenduke::qt::licensing::QtLicensingClient> licenses;

    tenduke::qt::demo::LicenseCheckoutWindow * licenseCheckout;
    tenduke::qt::demo::LoginDialog * loginDialog;
    tenduke::qt::demo::DemoParametersDialog* parametersDialog;
    tenduke::qt::demo::UserInfoWindow * userinfo;
};

}}}

#endif // MAINWINDOW_H
