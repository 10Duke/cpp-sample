#include "./testjwt.h"

#include "jwt/defaultjwtparser.h"
#include "jwt/fixedkeyjwtsignaturevalidator.h"
#include "crypto/asymmetricmessagedigestfactory.h"
#include "crypto/rsapublickeyfrompemstring.h"
#include "json/cjsonparser.h"
#include "utl/defaultbase64decoder.h"

#include <iostream>

namespace crypto = tenduke::crypto;
namespace libcrypto = tenduke::crypto::libcrypto;
namespace json = tenduke::json;
namespace cjson = tenduke::json::cjson;
namespace jwt = tenduke::jwt;
namespace utl = tenduke::utl;

static const char * RSA_KEY = "-----BEGIN PUBLIC KEY-----\n"
        "MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAywVSSuHKmyNrcT8JArxo\n"
        "IqTuWdCvG2R78p1Osdav8ivjQWqDnjR37tt7L+U+sopV4ka4gUQVi7Ie87l2cJwh\n"
        "sJ6uAQWfp6K7r+H/yH+ak+F8EvcWLFNqRjbvgAu0MqSt16bkZX01AanBca3yioZ+\n"
        "Ihe7DryKSbR1n8IMU7DRUiZzB4c9qdPphmDwxzryaiTkE1QJyXGjpSdvwwIdXE9u\n"
        "XE12zSeR2+CRKWTPZsnRBKpSDdrEwE8nSRW5XmDppnpoAvb6YI7ULtXZN354atbH\n"
        "sC1s+siHsjD7zB//cUzsRtge4YCTOoIs4thirizP3uXg8xJSs1Quie1GvZt0ufwl\n"
        "jMQnbBR7Le1ctV7sCZFom4XJGewGpnXQP9TBBpofH1RhjmBBRyruvbX3xGj2mKpi\n"
        "hy6k3FzoxZ580Pv1KGo1CYjLgfXFSmwnq/MJ6bE1wR9rmexOE1b2laWsTbTdpZB4\n"
        "/3mHGQ1yd5w+7ZjOQ1/K0g5FHm5yKK9cJSvQihN/BpGN5YhvwkpjhAhJlF+csLg4\n"
        "DGXl5GxnTfP1ZSUywOP2Da4PzpaghsDJpkBkh6rKDK+mJ9v0He1BfvhxIqAjVnur\n"
        "IRZriZ6mXwTM7C9v30IBIgnadgLkyptuj+/1F3Z3m2+Ix6uLpZGUQpWVgMcC2uM5\n"
        "kcU6rWjrfEfAVM23axgy7c0CAwEAAQ==\n"
        "-----END PUBLIC KEY-----\n";

void jwt::testJWT()
{
    std::shared_ptr<const json::JSONParser> jsonParser(new cjson::cJSONParser());
    std::shared_ptr<const utl::Base64Decoder> base64Decoder(new utl::DefaultBase64Decoder());
    std::shared_ptr<const crypto::PublicKey> verificationKey = libcrypto::RSAPublicKeyFromPEMString().from(RSA_KEY);
    std::shared_ptr<const crypto::MessageDigestVerifierFactory> verifierFactory(new libcrypto::AsymmetricMessageDigestFactory("sha256", verificationKey));
    std::shared_ptr<const jwt::JWTSignatureValidator> signatureValidator(new jwt::FixedKeyJWTSignatureValidator(verifierFactory));

    jwt::DefaultJWTParser parseJWT(
                jsonParser,
                base64Decoder,
                signatureValidator
    );

// Valid:
    jwt::JWT jwt = parseJWT.from("eyJhbGciOiJSUzI1NiJ9.eyJxdC1kZW1vLTFfZXJyb3JLZXkiOiJub3RBdXRob3JpemVkIiwicXQtZGVtby0xX2Vycm9yVGVjaG5pY2FsIjoiTm8gdmFsaWQgbGljZW5zZXMgZm91bmQuIiwicXQtZGVtby0xX2Vycm9yQ29kZSI6Im5vTGljZW5zZUZvdW5kIiwicXQtZGVtby0xX2Vycm9yTWVzc2FnZSI6IkFjY2VzcyB0byBcInF0LWRlbW8tMVwiIGRlbmllZCIsImV4cCI6MTY1MTU3NDgxNCwiaWF0IjoxNjUxNDg4NDE0fQ.iGUT0L3_ZmZSWiwj5EA2QpQSs5Ys4N4Vimev75c5MLdrleRi_vfAxFOwg3ZN0sH5vf_1J8xMPdIndHQGARwZi3sWg4GRBLy4hDUFvzo95UibhUFcxPr7VMjhhR_fgIheQpli6vXeKnw2QXEpSw1qXOfOO3L38UH9ZbyQQG05LSETC87n17Pfg9QHWv3vEvISQD0iY2XuYhB8kroFh9GHz49fYAsy0ootMZe3WVFP_Fb8oBlKKthQhWOZB8OHAZWrXFwtqAa5KGphOitKJAtqs8cBqbOcj7IXigBT8IHiX_MpLPBlGESCxBljQpq-EliFvZ5mZzrnpfpct31AD4bhhI0uLVuYjcVEcEzXhqIdwNZVwXStSpL2WTQcUgTFhDOiW4BH0NtlwVd_tg2a070FfPX2MSWiUL1Bj7lGxb5zPXReYoW477R18AY2EJjbRhW3HYXo1da5vNQhqIqwa4M4rJgQUkoHm6jfu925zMMJuLyMwsQXwNA18PW5pOTlliTYABEUdhLva0UaWVwLp_A-wjLKp9Cn7Obg4-NpSPWyDCx1Rx8KadPhMR4yLB8awUjEsSgz6rzVmV5Xqf4KJ0NwzKhMUoNq0mob3k6E1MH9psp6JrkxgSAP2AqcJ23_VwnhYbZGyFaeqD6prjjfeo9uLtOs1BZwIqMv8XDIganUBz0");
// Invalid signature (signature changed)
//    jwt::JWT jwt = parseJWT.from("eyJhbGciOiJSUzI1NiJ9.eyJxdC1kZW1vLTFfZXJyb3JLZXkiOiJub3RBdXRob3JpemVkIiwicXQtZGVtby0xX2Vycm9yVGVjaG5pY2FsIjoiTm8gdmFsaWQgbGljZW5zZXMgZm91bmQuIiwicXQtZGVtby0xX2Vycm9yQ29kZSI6Im5vTGljZW5zZUZvdW5kIiwicXQtZGVtby0xX2Vycm9yTWVzc2FnZSI6IkFjY2VzcyB0byBcInF0LWRlbW8tMVwiIGRlbmllZCIsImV4cCI6MTY1MTU3NDgxNCwiaWF0IjoxNjUxNDg4NDE0fQ.iGUT0L3_ZmZSWiwj5EA2QpQSs5Ys4N4Vimev75c5MLdrleRi_vfAxFOwg3ZN0sH5vf_1J8xMPdIndHQGARwZi3sWg4GRBLy4hDUFvzo95UibhUFcxPr7VMjhhR_fgIheQpli6vXeKnw2QXEpSw1qXOfOO3L38UH9ZbyQQG05LSETC87n17Pfg9QHWv3vEvISQD0iY2XuYhB8kroFh9GHz49fYAsy0ootMZe3WVFP_Fb8oBlKKthQhWOZB8OHAZWrXFwtqAa5KGphOitKJAtqs8cBqbOcj7IXigBT8IHiX_MpLPBlGESCxBljQpq-EliFvZ5mZzrnpfpct31AD4bhhI0uLVuYjcVEcEzXhqIdwNZVwXStSpL2WTQcUgTFhDOiW4BH0NtlwVd_tg2a070FfPX2MSWiUL1Bj7lGxb5zPXReYoW477R18AY2EJjbRhW3HYXo1da5vNQhqIqwa4M4rJgQUkoHm6jfu925zMMJuLyMwsQXwNA18PW5pOTlliTYABEUdhLva0UaWVwLp_A-wjLKp9Cn7Obg4-NpSPWyDCx1Rx8KadPhMR4yLB8awUjEsSgz6rzVmV5Xqf4KJ0NwzKhMUoNq0mob3k6E1MH9psp6JrkxgSAP2AqcJ23_VwnhYbZGyFaeqD6prjjfeo9uLtOs1BZwIqMv8XDIganUBz0");
// Invalid signature (header changed)
//    jwt::JWT jwt = parseJWT.from("eyJhbGciOiJTUzI1NiJ9.eyJxdC1kZW1vLTFfZXJyb3JLZXkiOiJub3RBdXRob3JpemVkIiwicXQtZGVtby0xX2Vycm9yVGVjaG5pY2FsIjoiTm8gdmFsaWQgbGljZW5zZXMgZm91bmQuIiwicXQtZGVtby0xX2Vycm9yQ29kZSI6Im5vTGljZW5zZUZvdW5kIiwicXQtZGVtby0xX2Vycm9yTWVzc2FnZSI6IkFjY2VzcyB0byBcInF0LWRlbW8tMVwiIGRlbmllZCIsImV4cCI6MTY1MTU3NDgxNCwiaWF0IjoxNjUxNDg4NDE0fQ.iGUT0L3_ZmZSWiwj5EA2QpQSs5Ys4N4Vimev75c5MLdrleRi_vfAxFOwg3ZN0sH5vf_1J8xMPdIndHQGARwZi3sWg4GRBLy4hDUFvzo95UibhUFcxPr7VMjhhR_fgIheQpli6vXeKnw2QXEpSw1qXOfOO3L38UH9ZbyQQG05LSETC87n17Pfg9QHWv3vEvISQD0iY2XuYhB8kroFh9GHz49fYAsy0ootMZe3WVFP_Fb8oBlKKthQhWOZB8OHAZWrXFwtqAa5KGphOitKJAtqs8cBqbOcj7IXigBT8IHiX_MpLPBlGESCxBljQpq-EliFvZ5mZzrnpfpct31AD4bhhI0uLVuYjcVEcEzXhqIdwNZVwXStSpL2WTQcUgTFhDOiW4BH0NtlwVd_tg2a070FfPX2MSWiUL1Bj7lGxb5zPXReYoW477R18AY2EJjbRhW3HYXo1da5vNQhqIqwa4M4rJgQUkoHm6jfu925zMMJuLyMwsQXwNA18PW5pOTlliTYABEUdhLva0UaWVwLp_A-wjLKp9Cn7Obg4-NpSPWyDCx1Rx8KadPhMR4yLB8awUjEsSgz6rzVmV5Xqf4KJ0NwzKhMUoNq0mob3k6E1MH9psp6JrkxgSAP2AqcJ23_VwnhYbZGyFaeqD6prjjfeo9uLtOs1BZwIqMv8XDIganUBz0");
// Invalid signature (body changed)
//    jwt::JWT jwt = parseJWT.from("eyJhbGciOiJSUzI1NiJ9.eyJRdC1kZW1vLTFfZXJyb3JLZXkiOiJub3RBdXRob3JpemVkIiwicXQtZGVtby0xX2Vycm9yVGVjaG5pY2FsIjoiTm8gdmFsaWQgbGljZW5zZXMgZm91bmQuIiwicXQtZGVtby0xX2Vycm9yQ29kZSI6Im5vTGljZW5zZUZvdW5kIiwicXQtZGVtby0xX2Vycm9yTWVzc2FnZSI6IkFjY2VzcyB0byBcInF0LWRlbW8tMVwiIGRlbmllZCIsImV4cCI6MTY1MTU3NDgxNCwiaWF0IjoxNjUxNDg4NDE0fQ.iGUT0L3_ZmZSWiwj5EA2QpQSs5Ys4N4Vimev75c5MLdrleRi_vfAxFOwg3ZN0sH5vf_1J8xMPdIndHQGARwZi3sWg4GRBLy4hDUFvzo95UibhUFcxPr7VMjhhR_fgIheQpli6vXeKnw2QXEpSw1qXOfOO3L38UH9ZbyQQG05LSETC87n17Pfg9QHWv3vEvISQD0iY2XuYhB8kroFh9GHz49fYAsy0ootMZe3WVFP_Fb8oBlKKthQhWOZB8OHAZWrXFwtqAa5KGphOitKJAtqs8cBqbOcj7IXigBT8IHiX_MpLPBlGESCxBljQpq-EliFvZ5mZzrnpfpct31AD4bhhI0uLVuYjcVEcEzXhqIdwNZVwXStSpL2WTQcUgTFhDOiW4BH0NtlwVd_tg2a070FfPX2MSWiUL1Bj7lGxb5zPXReYoW477R18AY2EJjbRhW3HYXo1da5vNQhqIqwa4M4rJgQUkoHm6jfu925zMMJuLyMwsQXwNA18PW5pOTlliTYABEUdhLva0UaWVwLp_A-wjLKp9Cn7Obg4-NpSPWyDCx1Rx8KadPhMR4yLB8awUjEsSgz6rzVmV5Xqf4KJ0NwzKhMUoNq0mob3k6E1MH9psp6JrkxgSAP2AqcJ23_VwnhYbZGyFaeqD6prjjfeo9uLtOs1BZwIqMv8XDIganUBz0");
// Invalid structure #1
//    jwt::JWT jwt = parseJWT.from("abc");
// Invalid structure #2
//    jwt::JWT jwt = parseJWT.from("abc.");
// Invalid structure #3
//    jwt::JWT jwt = parseJWT.from("abc.def");

    for (auto const &claim : jwt.getClaims()) {
        std::cout << claim.first << ": " << claim.second << "\n";
    }
}
