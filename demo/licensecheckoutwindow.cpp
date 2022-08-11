#include "./licensecheckoutwindow.h"
#include "./ui_licensecheckoutwindow.h"

#include "qt/licensing/qtlicensingclient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#include <iostream>

namespace qtdemo = tenduke::qt::demo;
namespace qtlic = tenduke::qt::licensing;
namespace xdlic = tenduke::licensing;

/** This class demonstrates how to use the credentials to perform API calls.
 *
 *
 */
qtdemo::LicenseCheckoutWindow::LicenseCheckoutWindow(
        const std::shared_ptr<qtlic::QtLicensingClient> license,
        QWidget *parent
) :
    BaseWindow(parent),
    ui(std::unique_ptr<Ui::LicenseCheckoutWindow>(new Ui::LicenseCheckoutWindow())),
    license(license),
    request(nullptr)
{
    ui->setupUi(this);

    QObject::connect(
        ui->btnCheckout, &QPushButton::clicked,
        this, &LicenseCheckoutWindow::checkoutLicense
    );
}

qtdemo::LicenseCheckoutWindow::~LicenseCheckoutWindow()
{
    // Intentionally empty
}

void qtdemo::LicenseCheckoutWindow::checkoutLicense()
{
    ui->btnCheckout->setEnabled(false);

    request = license->checkout(ui->licensedItemName->text());
    QObject::connect(
        request.get(), &qtlic::QtLicenseCheckoutRequest::finished,
        this, &LicenseCheckoutWindow::checkoutRequestFinished
    );
    QObject::connect(
        request.get(), &qtlic::QtLicenseCheckoutRequest::failed,
        this, &LicenseCheckoutWindow::checkoutRequestFailed
    );
    request->execute();
}

void qtdemo::LicenseCheckoutWindow::checkoutRequestFinished(xdlic::LicenseCheckoutResponse response)
{
    ui->btnCheckout->setEnabled(true);

    fillTable(response);

    request.reset(nullptr);
}


void qtdemo::LicenseCheckoutWindow::checkoutRequestFailed(std::exception_ptr error)
{
    // This demo should open some alert and show the error message there
    // but now just dumps the error to console.
    std::cerr << "The request failed:" << std::endl;
    try {
        std::rethrow_exception(error);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}


static QTableWidgetItem * gimmeCell(
        int row, int col, QTableWidget * table
) {
    QTableWidgetItem *cell = table->item(row, col);

    if (! cell) {
        cell = new QTableWidgetItem();
        table->setItem(row, col, cell);
    }

    return cell;
}


static void setCellText(
    QTableWidget * table,
    int row,
    int col,
    const QString &text
) {
    QTableWidgetItem *cell = gimmeCell(row, col, table);
    cell->setText(text);
}


void qtdemo::LicenseCheckoutWindow::fillTable(xdlic::LicenseCheckoutResponse &response)
{
    ui->tblLicense->clear();
    ui->tblLicense->setColumnCount(1);
    ui->tblLicense->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblLicense->setHorizontalHeaderLabels(QStringList("item"));

    int i = 0;
    for (auto const &item : response.getItems()) {
        ui->tblLicense->setRowCount(i + 1);
        setCellText(ui->tblLicense, i, 0, QString::fromStdString(item.first));
        i++;
    }

    ui->tblFailedItems->clear();
    ui->tblFailedItems->setColumnCount(3);
    ui->tblFailedItems->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblFailedItems->setHorizontalHeaderLabels(QStringList("item") << "code" << "message");

    i = 0;
    for (auto const &error : response.getFailedItems()) {
        ui->tblFailedItems->setRowCount(i + 1);
        setCellText(ui->tblFailedItems, i, 0, QString::fromStdString(error.first));
        setCellText(ui->tblFailedItems, i, 1, QString::fromStdString(error.second.getErrorCode()));
        setCellText(ui->tblFailedItems, i, 2, QString::fromStdString(error.second.getErrorMessage()));
        i++;
    }
}
