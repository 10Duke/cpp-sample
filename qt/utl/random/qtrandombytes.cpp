#include "qtrandombytes.h"

// This implementation uses QRandomGenerator because it is thread safe

#include <QRandomGenerator>

namespace qtutl = tenduke::qt::utl;

qtutl::QtRandomBytes::QtRandomBytes()
{
}


void qtutl::QtRandomBytes::generate(
        unsigned char * buffer,
        size_t bufferLength
) {
    while (bufferLength > 4) {
        quint32 value = QRandomGenerator::global()->generate();

        *buffer = value & 0xff;
        *(buffer + 1) = (value >> 8) & 0xff;
        *(buffer + 2) = (value >> 16) & 0xff;
        *(buffer + 3) = (value >> 24) & 0xff;

        buffer += 4;
        bufferLength -= 4;
    }

    if (bufferLength > 0) {
        quint32 value = QRandomGenerator::global()->generate();
        switch (bufferLength) {
            case 4:
                *buffer = value & 0xff;
                buffer++;
                value >>= 8;
                //fall through
            case 3:
                *buffer = value & 0xff;
                buffer++;
                value >>= 8;
                //fall through
            case 2:
                *buffer = value & 0xff;
                buffer++;
                value >>= 8;
                //fall through
            case 1:
                *buffer = value & 0xff;
                //fall through
            default:
                break;
        }
    }
}
