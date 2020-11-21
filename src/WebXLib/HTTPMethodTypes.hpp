#include <unordered_map>

#ifndef WEBX_HTTPMETHODTYPES_H
#define WEBX_HTTPMETHODTYPES_H

namespace WebXO
{
    enum class HTTPMethodTypes
    {
        GET,
        POST,
        HEAD,
        PUT,
        DELETE,
        CONNECT,
        OPTIONS,
        TRACE,
        PATCH
    };

    static std::unordered_map<std::string, HTTPMethodTypes> const HTTPMethodTypesTable = { 
        {"GET",HTTPMethodTypes::GET},
        {"POST",HTTPMethodTypes::POST}, 
        {"HEAD",HTTPMethodTypes::HEAD}, 
        {"PUT",HTTPMethodTypes::PUT}, 
        {"DELETE",HTTPMethodTypes::DELETE}, 
        {"CONNECT",HTTPMethodTypes::CONNECT}, 
        {"OPTIONS",HTTPMethodTypes::OPTIONS}, 
        {"TRACE",HTTPMethodTypes::TRACE}, 
        {"PATCH",HTTPMethodTypes::PATCH}, 
    };
}

#endif