#include "./createqtjwtparser.h"

#include "crypto/publickey.h"
#include "crypto/rsapublickeyfrompemstring.h"
#include "jwt/createjwtparser.h"
#include "qt/json/qtjsonparser.h"
#include "qt/utl/qtbase64decoder.h"

namespace libcrypto = tenduke::crypto::libcrypto;
namespace qtjson = tenduke::qt::json;
namespace qtjwt = tenduke::qt::jwt;
namespace qtutl = tenduke::qt::utl;
namespace xdcrypto = tenduke::crypto;
namespace xdjson = tenduke::json;
namespace xdjwt = tenduke::jwt;
namespace xdutl = tenduke::utl;

static std::unique_ptr<const xdcrypto::PublicKey> createPublicKey(QString key)
{
    if (key.isEmpty()) {
        return std::unique_ptr<const xdcrypto::PublicKey>();
    }

    // NOTE: This will throw CryptoException if the key is invalid.
    return libcrypto::RSAPublicKeyFromPEMString().from(key.toStdString());
}


std::unique_ptr<const tenduke::jwt::JWTParser> qtjwt::createQtJWTParser(
    const QString &algorithm,
    const QString &keyInPEMFormat
)
{
    std::shared_ptr<const xdutl::Base64Decoder> base64Decoder(new qtutl::QtBase64Decoder());
    std::shared_ptr<const xdjson::JSONParser> jsonParser(new qtjson::QtJSONParser());
    std::unique_ptr<const crypto::PublicKey> idTokenValidationKey = createPublicKey(keyInPEMFormat);

    return xdjwt::createJWTParser(
                algorithm.toStdString(),
                std::move(idTokenValidationKey),
                base64Decoder,
                jsonParser
    );
}

