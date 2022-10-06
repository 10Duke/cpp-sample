#ifndef TENDUKE_QT_JWT_CREATEQTJWTPARSER_H
#define TENDUKE_QT_JWT_CREATEQTJWTPARSER_H

#include "jwt/jwtparser.h"

#include <memory>

#include <QString>

namespace tenduke { namespace qt { namespace jwt {

std::unique_ptr<const tenduke::jwt::JWTParser> createQtJWTParser(
    const QString &algorithm,
    const QString &keyInPEMFormat
);

}}}

#endif // TENDUKE_QT_JWT_CREATEQTJWTPARSER_H
