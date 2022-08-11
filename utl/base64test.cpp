#include "./base64test.h"
#include "./defaultbase64decoder.h"
#include "./defaultbase64encoder.h"
#include <iostream>

namespace xdutl = tenduke::utl;

void xdutl::base64test()
{
    xdutl::DefaultBase64Decoder base64Decoder;

    std::cout << base64Decoder.decodeString("aGVsbG8sIHdvcmxkIQo=") << "\n";
    std::cout << base64Decoder.decodeString("OT/DpDk+w6Q=") << "\n";
    std::cout << base64Decoder.decodeString("OT_DpDk-w6Q") << "\n";

    xdutl::DefaultBase64Encoder base64Encoder;
    std::cout << base64Encoder.encode("hello, world!") << "\n";
    std::cout << base64Encoder.encodeUrlSafe("hello, world!") << "\n";
    std::cout << base64Encoder.encode("9?ä9>ä") << "\n";
    std::cout << base64Encoder.encodeUrlSafe("9?ä9>ä") << "\n";
}
