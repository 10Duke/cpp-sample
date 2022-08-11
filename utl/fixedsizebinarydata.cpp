#include "fixedsizebinarydata.h"

namespace utl = tenduke::utl;

utl::FixedSizeBinaryData::FixedSizeBinaryData(const size_t length)
    : data(length == 0 ? nullptr : (new unsigned char [length])), length(length)
{
}

utl::FixedSizeBinaryData::FixedSizeBinaryData(
    std::unique_ptr<unsigned char> data,
    const size_t length
)
    : data(std::move(data)), length(length)
{
}

utl::FixedSizeBinaryData::~FixedSizeBinaryData()
{
}

unsigned char * utl::FixedSizeBinaryData::getData() const
{
    return data.get();
}

size_t utl::FixedSizeBinaryData::getLength() const
{
    return length;
}

