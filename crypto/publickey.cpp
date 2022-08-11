#include "./publickey.h"

#include <openssl/evp.h>

namespace crypto = tenduke::crypto;

crypto::PublicKey::PublicKey(EVP_PKEY *key)
    : key(key)
{
}

crypto::PublicKey::~PublicKey()
{
    EVP_PKEY_free(this->key);
}
