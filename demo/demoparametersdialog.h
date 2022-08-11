#ifndef DEMOPARAMETERSDIALOG_H
#define DEMOPARAMETERSDIALOG_H

#include "./basewindow.h"

#include "qt/licensing/qtlicensingconfiguration.h"
#include "qt/oidc/qtoidcconfig.h"

namespace tenduke { namespace qt { namespace demo {

namespace Ui {
class DemoParametersDialog;
}

class DemoParametersDialog : public tenduke::qt::demo::BaseWindow
{
    Q_OBJECT

public:
    explicit DemoParametersDialog(
            QWidget *parent = nullptr
    );
    ~DemoParametersDialog();

Q_SIGNALS:
    void parametersCollected(
            tenduke::qt::oidc::QtOIDCConfig oidcParameters,
            tenduke::qt::licensing::QtLicensingConfiguration licensingConfiguration
    );

public slots:
    void accept();

private:
    Ui::DemoParametersDialog *ui;
};

}}}

#endif // DEMOPARAMETERSDIALOG_H
