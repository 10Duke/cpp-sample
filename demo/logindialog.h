#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "demo/basewindow.h"
#include "http/httpclient.h"
#include "qt/webengine/customschemehandler.h"
#include "qt/oidc/qtoidcclient.h"
#include "qt/oidc/qtoidcstate.h"
#include "qt/oidc/qtoidcconfig.h"

#include <memory>

#include <QWebEngineView>

namespace tenduke { namespace qt { namespace demo {

/**
 *  This is a login dialog with embedded browser.
 *
 */
class LoginDialog : public tenduke::qt::demo::BaseWindow
{
    Q_OBJECT

public:
    explicit LoginDialog(
            std::shared_ptr<tenduke::qt::webengine::CustomSchemeHandler> schemeHandler,
            std::shared_ptr<tenduke::qt::oidc::QtOIDCClient> oidc,
            QWidget *parent = nullptr
    );
    ~LoginDialog();

    QWebEngineView *getBrowser();
    void start();

Q_SIGNALS:
    void canceled();
    void loginSuccessful(tenduke::qt::oidc::QtOIDCState state);
    void loginFailed(std::exception_ptr error);

private:
    QWebEngineView browser;
    const std::shared_ptr<tenduke::qt::webengine::CustomSchemeHandler> schemeHandler;
    const std::shared_ptr<tenduke::qt::oidc::QtOIDCClient> oidc;

    void disconnectSignals();
};

}}}

#endif // LOGINDIALOG_H
