#ifndef SIMPLEBUFFER_H
#define SIMPLEBUFFER_H

#include "./binarydata.h"

namespace tenduke { namespace utl {


/** Simple extensible binary buffer.
 *
 */
class SimpleBuffer : public BinaryData
{
public:
    SimpleBuffer();
    virtual ~SimpleBuffer();

    /** Appends data to the buffer.
     *
     * @param newData data to append
     * @param newDataLength length of the data to append in bytes.
     */
    virtual void append(unsigned char *newData, size_t newDataLength);

    // tenduke::utl::BinaryData interface
    virtual unsigned char * getData() const override {return data;}
    virtual size_t getLength() const override {return length;}

private:
    unsigned char * data;
    size_t length;
};


}}

#endif // SIMPLEBUFFER_H
