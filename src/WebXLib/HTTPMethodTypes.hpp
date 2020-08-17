#ifndef WEBX_HTTPMETHODTYPES_H
#define WEBX_HTTPMETHODTYPES_H

namespace WebX
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
}

#endif