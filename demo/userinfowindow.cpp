#include "./userinfowindow.h"
#include "./ui_userinfowindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

namespace qtdemo = tenduke::qt::demo;
namespace qtoauth = tenduke::qt::oauth;

/** This class demonstrates how to use the credentials to perform API calls.
 *
 *
 */
qtdemo::UserInfoWindow::UserInfoWindow(
        const std::shared_ptr<QNetworkAccessManager> networkAccessManager,
        const QUrl userinfoEndpoint,
        const std::shared_ptr<const qtoauth::QtOAuthState> oidcState,
        QWidget *parent
) :
    BaseWindow(parent),
    ui(std::unique_ptr<Ui::UserInfoWindow>(new Ui::UserInfoWindow())),
    networkAccessManager(networkAccessManager),
    userinfoEndpoint(userinfoEndpoint),
    oidcState(oidcState)
{
    ui->setupUi(this);

    QObject::connect(
        ui->btnLoadUserinfo, &QPushButton::clicked,
        this, &UserInfoWindow::loadUserinfo
    );

}

qtdemo::UserInfoWindow::~UserInfoWindow()
{
    // Intentionally empty: Needed for unique_ptr...
}

void qtdemo::UserInfoWindow::loadUserinfo()
{
    QNetworkRequest request (userinfoEndpoint);
    QString header = "Bearer ";

    header += oidcState->getAccessToken();
    request.setRawHeader(QByteArray("Authorization"), header.toUtf8());

    auto reply = networkAccessManager->get(request);

    QObject::connect(
                reply,
                &QNetworkReply::finished,
                [=]()
    {
        if (reply->error() == QNetworkReply::NoError) {
            const auto responseBody = reply->readAll();
            const auto json = QJsonDocument::fromJson(responseBody);
            const auto data = json.object();
            populateTable(ui->tblUserinfo, &data);
        }
        else {
            // TODO: Handle error
        }
        reply->deleteLater();
    });
}


void qtdemo::UserInfoWindow::populateTable(QTableWidget *table, const QJsonObject *json)
{
    table->clear();
    table->setRowCount(json->keys().size());
    table->setColumnCount(2);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setHorizontalHeaderLabels(QStringList("key") << "value");

    for (int i = 0; i < json->keys().size(); i++) {

        const QString jsonKey = json->keys().at(i);
        const QJsonValue jsonValue = json->value(jsonKey);

        QTableWidgetItem *fieldName = table->item(i, 0);
        QTableWidgetItem *fieldValue = table->item(i, 1);

        if(!fieldName)
        {
            fieldName = new QTableWidgetItem;
            table->setItem(i, 0, fieldName);
        }

        if(!fieldValue)
        {
            fieldValue = new QTableWidgetItem;
            table->setItem(i, 1, fieldValue);
        }

        fieldName->setText(jsonKey);
        if (jsonValue.isString()) {
            fieldValue->setText(jsonValue.toString());
        } else if (jsonValue.isBool()) {
            fieldValue->setText(jsonValue.toBool() ? "true" : "false");
        } else {
            fieldValue->setText(QString::number(jsonValue.toInt()));
        }
    }

}
