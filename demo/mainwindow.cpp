#include "./mainwindow.h"
#include "./ui_mainwindow.h"
#include "./logindialog.h"

#include "qt/http/createqthttpclient.h"
#include "qt/licensing/createqtlicensingclient.h"
#include "qt/oidc/createqtoidcclient.h"

#include <iostream>


namespace qtdemo = tenduke::qt::demo;
namespace qthttp = tenduke::qt::http;
namespace qtlic = tenduke::qt::licensing;
namespace qtoauth = tenduke::qt::oauth;
namespace qtoidc = tenduke::qt::oidc;
namespace xdhttp = tenduke::http;


qtdemo::MainWindow::MainWindow(
        const std::shared_ptr<tenduke::qt::webengine::CustomSchemeHandler> schemeHandler,
        const std::shared_ptr<QNetworkAccessManager> networkAccessManager,
        QWidget *parent
) : QMainWindow(parent),
    ui(std::unique_ptr<Ui::MainWindow>(new Ui::MainWindow())),
    licensingParameters(nullptr), oidcParameters(nullptr), oidcState(nullptr),
    schemeHandler(schemeHandler), networkAccessManager(networkAccessManager),
    httpClient(qthttp::createQtHTTPClient(networkAccessManager)), oidc(nullptr), licenses(nullptr),
    licenseCheckout(nullptr), loginDialog(nullptr), parametersDialog(nullptr), userinfo(nullptr)
{
    ui->setupUi(this);
    ui->btnRefreshAccessToken->setEnabled(false);

    QObject::connect(
        ui->btnLogin, &QPushButton::clicked,
        this, &MainWindow::openParametersDialog
    );

    QObject::connect(
        ui->btnLoadUserInfo, &QPushButton::clicked,
        this, &MainWindow::openUserInfo
    );

    QObject::connect(
        ui->btnCheckoutLicenses, &QPushButton::clicked,
        this, &MainWindow::openLicenseWindow
    );

    QObject::connect(
        ui->btnRefreshAccessToken, &QPushButton::clicked,
        this, &MainWindow::refreshAccessToken
    );
}


qtdemo::MainWindow::~MainWindow()
{
    closeAndRemoveLicenseWindow();
    closeAndRemoveLoginDialog();
    closeAndRemoveParametersDialog();
    closeAndRemoveUserInfoDialog();
}


void qtdemo::MainWindow::openParametersDialog()
{
    // ERROR: Dialog already open
    if (parametersDialog != nullptr) {
        std::cerr<<"MainWindow::openParametersDialog() - ALREADY OPEN!\n";
        return;
    }

    parametersDialog = new qtdemo::DemoParametersDialog(this);

    QObject::connect(
            parametersDialog, &qtdemo::DemoParametersDialog::parametersCollected,
            this, &qtdemo::MainWindow::parametersReady
    );
    QObject::connect(
            parametersDialog, &qtdemo::DemoParametersDialog::closed,
            this, &qtdemo::MainWindow::closeAndRemoveParametersDialog
    );

    parametersDialog->show();
}


/** Slot, called when user has entered parameters. Opens Login-dialog.
 *
 */
void qtdemo::MainWindow::parametersReady(
        qtoidc::QtOIDCConfig oidcParameters,
        qtlic::QtLicensingConfiguration licensingParameters
)
{
    parametersDialog->close();

    this->oidcParameters.reset(new qtoidc::QtOIDCConfig(oidcParameters));
    this->licensingParameters.reset(new qtlic::QtLicensingConfiguration(licensingParameters));

    login();
}

void qtdemo::MainWindow::login()
{
    // ERROR: No parameters
    if (oidcParameters == nullptr) {
        std::cerr<<"MainWindow::login() - NO PARAMETERS!\n";
        return;
    }
    // ERROR: Dialog already open
    if (loginDialog != nullptr) {
        std::cerr<<"MainWindow::login() - ALREADY OPEN!\n";
        return;
    }

    this->oidc = qtoidc::createOidcClient(
        oidcParameters,
        httpClient
    );

    // Open the login-dialog:
    loginDialog = new LoginDialog(
        schemeHandler,
        oidc,
        this
    );

    QObject::connect(
        loginDialog, &LoginDialog::loginSuccessful,
        this, &MainWindow::loginSuccessful
    );
    QObject::connect(
        loginDialog, &LoginDialog::canceled,
        loginDialog, &LoginDialog::close
    );
    QObject::connect(
        loginDialog, &LoginDialog::closed,
        this, &MainWindow::closeAndRemoveLoginDialog
    );
    QObject::connect(
        loginDialog, &LoginDialog::loginFailed,
        this, &MainWindow::loginFailed
    );

    loginDialog->start();
}


void qtdemo::MainWindow::loginSuccessful(qtoidc::QtOIDCState state)
{
    loginDialog->close();
    oidcState.reset(new qtoidc::QtOIDCState(state));

    // Enable refresh-button:
    ui->btnRefreshAccessToken->setEnabled(true);

    // Setup new licensing client:
    if (licenses) {
        QObject::disconnect(
            oidc.get(), &qtoidc::QtOIDCClient::authenticated,
            licenses.get(), &qtlic::QtLicensingClient::oauthStateRefreshed
        );
        QObject::disconnect(
            oidc.get(), &qtoidc::QtOIDCClient::authenticationFailure,
            this, &qtdemo::MainWindow::loginFailed
        );
    }

    licenses = qtlic::createQtLicensingClient(
        *(licensingParameters.get()),
        oidcState->getAccessToken().toStdString(),
        httpClient
    );

    // Bind signals. These are emitted from access-token refresh:
    QObject::connect(
        oidc.get(), &qtoidc::QtOIDCClient::authenticated,
        licenses.get(), &qtlic::QtLicensingClient::oauthStateRefreshed
    );
    QObject::connect(
        oidc.get(), &qtoidc::QtOIDCClient::authenticationFailure,
        this, &qtdemo::MainWindow::accessTokenRefreshFailed
    );
}


void qtdemo::MainWindow::loginFailed(std::exception_ptr error)
{
    loginDialog->close();

    std::cerr << "Login failed:" << std::endl;

    // This demo should show some sort of alert here to indicate that the login failed.
    // The failure can happen in the browser-authentication part or in then
    // finalization part (where code gets exchanged for access token).
    // Here we simply print the error to console.

    // IMPORTANT NOTE: Some exceptions are retryable conditions, some are not.
    // For example tenduke::net::UnknownHostException is very likely caused by invalid configuration
    // in which case it is futile to re-try the action. Some conditions are potentially re-tryable.

    // IMPORTANT NOTE: The exceptions thrown should be documented in the service, in login-failure
    // case, in tenduke::qt::oidc::QtOIDCClient
    if (error) {
        try {
            std::rethrow_exception(error);
        }
        catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}


void qtdemo::MainWindow::openUserInfo()
{
    // ERROR: No parameters
    if (oidcParameters == nullptr) {
        return;
    }

    // ERROR: Window already open
    if (userinfo != nullptr) {
        return;
    }

    // Open the window:
    userinfo = new UserInfoWindow(
        networkAccessManager,
        oidcParameters->getUserinfoEndpoint(),
        oidcState
    );

    QObject::connect(
        userinfo, &UserInfoWindow::closed,
        this, &MainWindow::closeAndRemoveUserInfoDialog
    );

    userinfo->show();
}


static void closeWindow(qtdemo::BaseWindow ** window)
{
    if (*window != nullptr) {
        if (! (*window)->isCloseEventEmitted()) {
            (*window)->close();
        }
        (*window)->deleteLater();
        *window = nullptr;
    }
}


void qtdemo::MainWindow::openLicenseWindow()
{
    // ERROR: Not logged in
    if (oidcState == nullptr) {
        return;
    }

    // ERROR: Already open
    if (licenseCheckout != nullptr) {
        return;
    }

    licenseCheckout = new qtdemo::LicenseCheckoutWindow(
        licenses,
        this
    );

    QObject::connect(
        licenseCheckout, &qtdemo::LicenseCheckoutWindow::closed,
        this, &qtdemo::MainWindow::closeAndRemoveLicenseWindow
    );

    licenseCheckout->show();
}


void qtdemo::MainWindow::refreshAccessToken()
{
    if (oidc == nullptr) {
        std::cerr << "Cannot refresh access token, no OIDC yet";
        return;
    }

    if (! oidc->isAuthenticated()) {
        std::cerr << "Cannot refresh access token, no authenticated state: Nothing to refresh";
        return;
    }

    // NOTE: Refresh triggers signal authenticated(), which is already connected to QtLicensingClient::oauthStateRefreshed()
    //       Failure trigges authenticationFailure(), which is already connected to MainWindow::loginFailure()
    oidc->refresh();
}


void qtdemo::MainWindow::accessTokenRefreshFailed(std::exception_ptr error)
{
    std::cerr << "Token refresh failed:" << std::endl;

    // In this demo we just dump the error message.
    // In a real application, you might want to do something else.
    // Refresh can fail for several reasons, but most common are a) the entire OAuth-state has expired
    // and user has to be completely re-authenticated and b) user account is disabled / removed
    // Those two cases are not retryable.
    if (error) {
        try {
            std::rethrow_exception(error);
        }
        catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}


void qtdemo::MainWindow::closeAndRemoveLicenseWindow()
{
    closeWindow((demo::BaseWindow**)&(this->licenseCheckout));
}


void qtdemo::MainWindow::closeAndRemoveLoginDialog()
{
    closeWindow((demo::BaseWindow**)&(this->loginDialog));
}


void qtdemo::MainWindow::closeAndRemoveParametersDialog()
{
    closeWindow((demo::BaseWindow**)&(this->parametersDialog));
}


void qtdemo::MainWindow::closeAndRemoveUserInfoDialog()
{
    closeWindow((demo::BaseWindow**)&(this->userinfo));
}

