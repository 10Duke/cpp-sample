#ifndef TENDUKE_QT_UTL_QT_DELETELATERFUNCTION_H
#define TENDUKE_QT_UTL_QT_DELETELATERFUNCTION_H


#include <QObject>

namespace tenduke { namespace qt { namespace utl {

struct QtDeleteLaterFunction
{
    void operator()(QObject* object) { object->deleteLater(); }
};

}}}

#endif // TENDUKE_QT_UTL_QT_DELETELATERFUNCTION_H
