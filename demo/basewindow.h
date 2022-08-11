#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>

namespace tenduke { namespace qt { namespace demo {

class BaseWindow : public QMainWindow
{
    Q_OBJECT

public:
    BaseWindow(QWidget * parent = nullptr);
    virtual ~BaseWindow() {}

    virtual bool isCloseEventEmitted();

Q_SIGNALS:
    void closed();


protected:
    virtual void closeEvent( QCloseEvent* event );

private:
    bool closeEventEmitted;
};

}}}

#endif // BASEWINDOW_H
