#include "./logindialog.h"


namespace qtdemo = tenduke::qt::demo;
namespace qtoidc = tenduke::qt::oidc;
namespace qtwe = tenduke::qt::webengine;


qtdemo::LoginDialog::LoginDialog(
        std::shared_ptr<qtwe::CustomSchemeHandler> schemeHandler,
        std::shared_ptr<qtoidc::QtOIDCClient> oidc,
        QWidget *parent
) : qtdemo::BaseWindow(parent), browser(this), schemeHandler(schemeHandler), oidc(oidc)
{
    this->setCentralWidget(&browser);
}


qtdemo::LoginDialog::~LoginDialog()
{
    disconnectSignals();
}


QWebEngineView * qtdemo::LoginDialog::getBrowser()
{
    return &browser;
}


void qtdemo::LoginDialog::start()
{
    // When the OAuth-flow wants to open browser, instruct the embedded browser
    // to load the authorization URL:
    QObject::connect(
                oidc.get(), &qtoidc::QtOIDCClient::openBrowser,
                &browser, &QWebEngineView::setUrl
    );

    // When the callback URL is navigated to, the CustomSchemeHandler emits
    // customUrlNavigated()-signal. This is connected to the OAuth flow to
    // so that it can continue:
    QObject::connect(
                schemeHandler.get(), &qtwe::CustomSchemeHandler::customUrlNavigated,
                oidc.get(), &qtoidc::QtOIDCClient::callbackUrlNavigated
    );

    // Relay OIDC authentication succeess
    QObject::connect(
                oidc.get(), &qtoidc::QtOIDCClient::authenticated,
                this, &qtdemo::LoginDialog::loginSuccessful
    );

    // Relay exception via signal on failure
    QObject::connect(
                oidc.get(), &qtoidc::QtOIDCClient::authenticationFailure,
                this, &qtdemo::LoginDialog::loginFailed
    );

    resize(1024, 768);
    show();

    // Starts SSO-flow:
    // - opens browser & navigates to authentication URL
    // - once authentication complete in browser, returns to handle callback URL
    // - executes next steps to complete the authentication
    // - signals oidcLoginSucceeded() once everything ready
    oidc->authenticate(QString("profile email"));
}


void qtdemo::LoginDialog::disconnectSignals()
{
    // Disconnect the signal handlers
    QObject::disconnect(
                oidc.get(), &qtoidc::QtOIDCClient::authenticationFailure,
                this, &qtdemo::LoginDialog::loginFailed
    );
    QObject::disconnect(
                oidc.get(), &qtoidc::QtOIDCClient::authenticated,
                this, &qtdemo::LoginDialog::loginSuccessful
    );
    QObject::disconnect(
                schemeHandler.get(), &qtwe::CustomSchemeHandler::customUrlNavigated,
                oidc.get(), &qtoidc::QtOIDCClient::callbackUrlNavigated
    );
    QObject::disconnect(
                oidc.get(), &qtoidc::QtOIDCClient::openBrowser,
                &browser, &QWebEngineView::setUrl
    );
}
