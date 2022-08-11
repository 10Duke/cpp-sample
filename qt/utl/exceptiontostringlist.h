#ifndef TENDUKE_QT_UTL_EXCEPTIONTOSTRINGLIST_H
#define TENDUKE_QT_UTL_EXCEPTIONTOSTRINGLIST_H

#include <QStringList>

namespace tenduke { namespace qt { namespace utl {


/** Unwraps exception, potentially nested (std::nested_exception), into a QStringList,
 *  where each item is the std::exception::what(). The first item is the top most exception
 *  and last is the root cause.
 *
 *  @param -
 *  @return -
 */
QStringList exceptionToStringList(const std::exception &e);


}}}

#endif // TENDUKE_QT_UTL_EXCEPTIONTOSTRINGLIST_H
