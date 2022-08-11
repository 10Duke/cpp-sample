#ifndef BINARYDATA_H
#define BINARYDATA_H

#include <stddef.h>

namespace tenduke { namespace utl {


/** A holder of binary data.
 *
 */
class BinaryData
{
public:
    virtual ~BinaryData() {}

    /** Returns the data.
     *  DO NOT free() or delete() this: The class maintains the data.
     *  The data is mutable.
     */
    virtual unsigned char * getData() const = 0;

    /** Returns the length of data in bytes.
     */
    virtual size_t getLength() const = 0;
};


}}

#endif // BINARYDATA_H
