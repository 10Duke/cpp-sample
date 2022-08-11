#ifndef USERINFOWINDOW_H
#define USERINFOWINDOW_H

#include "./basewindow.h"

#include <memory>

#include <QNetworkAccessManager>
#include <QTableWidget>
#include <QUrl>
#include "qt/oauth/qtoauthstate.h"

namespace tenduke { namespace qt { namespace demo {

namespace Ui {
class UserInfoWindow;
}

class UserInfoWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit UserInfoWindow(
            const std::shared_ptr<QNetworkAccessManager> networkAccessManager,
            const QUrl userinfoEndpoint,
            const std::shared_ptr<const tenduke::qt::oauth::QtOAuthState> oidcState,
            QWidget *parent = nullptr
    );
    ~UserInfoWindow();

private:
    void loadUserinfo();
    void populateTable(QTableWidget * table, const QJsonObject *json);

    const std::unique_ptr<Ui::UserInfoWindow>  ui;

    const std::shared_ptr<QNetworkAccessManager> networkAccessManager;
    const QUrl userinfoEndpoint;
    const std::shared_ptr<const tenduke::qt::oauth::QtOAuthState> oidcState;
};

}}}

#endif // USERINFOWINDOW_H
