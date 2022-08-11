#include "./basewindow.h"

namespace qtdemo = tenduke::qt::demo;

qtdemo::BaseWindow::BaseWindow(QWidget * parent)
    : QMainWindow(parent), closeEventEmitted(false)
{
}

void qtdemo::BaseWindow::closeEvent(QCloseEvent * event)
{
    Q_EMIT closed();
    QMainWindow::closeEvent(event);
    closeEventEmitted = true;
}

bool qtdemo::BaseWindow::isCloseEventEmitted()
{
    return closeEventEmitted;
}
