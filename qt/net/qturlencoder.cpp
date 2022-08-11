#include "qturlencoder.h"

#include <QByteArray>

namespace qtnet = tenduke::qt::net;

std::string qtnet::QtURLEncoder::encode(const std::string &stringToEncode) const
{
    return QByteArray::fromStdString(stringToEncode).toPercentEncoding().toStdString();
}

