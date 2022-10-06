#ifndef TENDUKE_JWT_CREATEJWTPARSER_H
#define TENDUKE_JWT_CREATEJWTPARSER_H

#include "./jwtparser.h"

#include "crypto/publickey.h"
#include "json/jsonparser.h"
#include "utl/base64decoder.h"

#include <memory>
#include <string>


namespace tenduke { namespace jwt {

/** Factory method for creating default JWT parser.
 */
std::unique_ptr<const JWTParser> createJWTParser(
    const std::string &algorithm,
    std::shared_ptr<const tenduke::crypto::PublicKey> verificationKey,
    std::shared_ptr<const tenduke::utl::Base64Decoder> base64Decoder,
    std::shared_ptr<const tenduke::json::JSONParser> jsonParser
);

}}

#endif // TENDUKE_JWT_CREATEJWTPARSER_H
