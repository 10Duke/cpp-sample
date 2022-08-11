#ifndef PUBLICKEY_H
#define PUBLICKEY_H

#include <openssl/ossl_typ.h>

namespace tenduke { namespace crypto {


/** Libcrypto-implementaiton of a public key definition.
 *
 */
class PublicKey
{
public:
    PublicKey(EVP_PKEY * key);
    ~PublicKey();

    EVP_PKEY * getKey() const {return key;}

private:
    EVP_PKEY * key;
};


}}

#endif // PUBLICKEY_H
