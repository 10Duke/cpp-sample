#include "exceptiontostringlist.h"

static void unwrap(const std::exception &e, QStringList &stack)
{
    stack.push_back(QString::fromStdString(e.what()));
    try {
        std::rethrow_if_nested(e);
    } catch(const std::exception& nestedException) {
        unwrap(nestedException, stack);
    }
}

QStringList tenduke::qt::utl::exceptionToStringList(const std::exception &e)
{
    QStringList stack;

    unwrap(e, stack);

    return stack;
}
