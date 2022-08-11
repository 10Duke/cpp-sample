#include "httpresponse.h"


namespace http = tenduke::http;

http::HTTPResponse::HTTPResponse(
        int statusCode,
        std::multimap<std::string, std::string> headers,
        std::unique_ptr<tenduke::utl::BinaryData> data
) : statusCode(statusCode), headers(headers), data(std::move(data))
{
}


bool http::HTTPResponse::isSuccessful()
{
    return (statusCode < 400);
}


std::string http::HTTPResponse::getPayloadAsString()
{
    return (std::string((char*)(data->getData()), data->getLength()));
}


size_t http::HTTPResponse::getPayloadLength()
{
    return data->getLength();
}
