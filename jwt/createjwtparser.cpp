#include "./createjwtparser.h"

#include "./defaultjwtparser.h"
#include "./fixedkeyjwtsignaturevalidator.h"

#include "crypto/asymmetricmessagedigestfactory.h"

namespace xdcrypto = tenduke::crypto;
namespace xdjson = tenduke::json;
namespace xdjwt = tenduke::jwt;
namespace xdutl = tenduke::utl;
namespace libcrypto = tenduke::crypto::libcrypto;

std::unique_ptr<const xdjwt::JWTParser> xdjwt::createJWTParser(
    const std::string &algorithm,
    std::shared_ptr<const xdcrypto::PublicKey> verificationKey,
    std::shared_ptr<const xdutl::Base64Decoder> base64Decoder,
    std::shared_ptr<const xdjson::JSONParser> jsonParser
)
{
    std::shared_ptr<const crypto::MessageDigestVerifierFactory> digestFactory(new libcrypto::AsymmetricMessageDigestFactory(
        algorithm,
        verificationKey
    ));
    std::shared_ptr<const xdjwt::JWTSignatureValidator> jwtSignatureValidator (new xdjwt::FixedKeyJWTSignatureValidator(digestFactory));

    return std::unique_ptr<const xdjwt::JWTParser>(new jwt::DefaultJWTParser(
                jsonParser,
                base64Decoder,
                jwtSignatureValidator
    ));
}
