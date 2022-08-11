#include "demoparametersdialog.h"
#include "ui_demoparametersdialog.h"

namespace qtdemo = tenduke::qt::demo;
namespace qtlic = tenduke::qt::licensing;

qtdemo::DemoParametersDialog::DemoParametersDialog(QWidget *parent) :
    qtdemo::BaseWindow(parent),
    ui(new Ui::DemoParametersDialog)
{
    ui->setupUi(this);
    QObject::connect(
        ui->btnOK, &QPushButton::clicked,
        this, &qtdemo::DemoParametersDialog::accept
    );
}

qtdemo::DemoParametersDialog::~DemoParametersDialog()
{
    delete ui;
}

void qtdemo::DemoParametersDialog::accept()
{
    Q_EMIT parametersCollected(
        tenduke::qt::oidc::QtOIDCConfig (
            QUrl(ui->authorizationEndpoint->text()),
            QUrl(ui->tokenEndpoint->text()),
            QUrl(ui->userinfoEndpoint->text()),
            ui->redirectUri->text(),
            ui->clientIdentifier->text(),
            ui->clientSecret->text(),
            ui->issuer->text(),
            ui->verificationKey->toPlainText()
        ),
        qtlic::QtLicensingConfiguration(
            QUrl(ui->licenseEndpoint->text()),
            ui->hardwareId->text()
        )
    );
}
