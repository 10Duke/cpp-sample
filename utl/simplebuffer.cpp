#include "simplebuffer.h"

#include <new>
#include <stdlib.h>
#include <string.h>

namespace utl = tenduke::utl;

utl::SimpleBuffer::SimpleBuffer()
    : data(nullptr), length(0)
{
}

utl::SimpleBuffer::~SimpleBuffer()
{
    if (data != nullptr) {
        free(data);
    }
}

void utl::SimpleBuffer::append(unsigned char * newData, size_t newDataLength)
{
    const int index = length; // Index where the new data is to be copied to
    length += newDataLength;

    // Use unsigned char to indicate contents are bytes
    unsigned char * newBuffer = (unsigned char *) realloc(data, length);
    if (newBuffer == nullptr) {
        // NOTE: the old data is free()d by the destructor
        throw std::bad_alloc();
    }
    data = newBuffer;
    memcpy(newBuffer + index, newData, newDataLength);
}
