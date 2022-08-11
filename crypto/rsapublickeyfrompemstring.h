#ifndef RSAPUBLICKEYFROMPEMSTRING_H
#define RSAPUBLICKEYFROMPEMSTRING_H

#include "publickey.h"

#include <memory>
#include <string>

namespace tenduke { namespace crypto { namespace libcrypto {


/** Utility for creating RSA public key from PEM-string
 *
 */
class RSAPublicKeyFromPEMString
{
public:
    RSAPublicKeyFromPEMString();

    /** Creates RSA PublicKey from PEM-format string.
     *
     * @param string the RSA public key in PEM-format
     * @return -
     */
    std::unique_ptr<const PublicKey> from(const std::string &string);
};


}}}

#endif // RSAPUBLICKEYFROMPEMSTRING_H
