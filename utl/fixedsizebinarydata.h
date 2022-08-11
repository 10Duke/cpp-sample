#ifndef FIXEDSIZEBINARYDATA_H
#define FIXEDSIZEBINARYDATA_H

#include "./binarydata.h"

#include <memory>

namespace tenduke { namespace utl {


/* Utility class for fixed size binary data.
 *
 */
class FixedSizeBinaryData : public tenduke::utl::BinaryData
{
public:
    /** Constructs new instance by allocating memory of {@code length} bytes.
     *
     *  @param length num bytes to allocate.
     */
    FixedSizeBinaryData(const size_t length);

    /** Constructs new instance by taking ownership of given data.
     *
     *  @param data the data to wrap
     *  @param length size of the data
     */
    FixedSizeBinaryData(std::unique_ptr<unsigned char> data, const size_t length);

    /** Desctructs the instance, frees contained data.
     */
    virtual ~FixedSizeBinaryData();

// BinaryData interface
public:
    /** Returns pointer to the data. Do not free or deallocate this!
     *
     *  @return -
     */
    virtual unsigned char *getData() const override;

    /** Returns length of the data.
     *
     *  @return -
     */
    virtual size_t getLength() const override;

private:
    const std::unique_ptr<unsigned char> data;
    size_t length;
};


}}

#endif // FIXEDSIZEBINARYDATA_H
