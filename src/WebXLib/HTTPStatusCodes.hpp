#ifndef WEBX_HTTPSTATUSCODES_H
#define WEBX_HTTPSTATUSCODES_H

namespace WebXO
{
    enum class HTTPStatusCodes
    {
        // 1xx Status Codes
        // 2xx Status Codes
        OK                      = 200,
        NO_CONTENT              = 204,
        // 3xx Status Codes
        // 4xx Status Codes
        BAD_REQUEST             = 400,
        UNAUTHORIZED            = 401,
        FORBIDDEN               = 403,
        NOT_FOUND               = 404,
        METHOD_NOT_ALLOWED      = 405,
        NOT_ACCEPTED            = 406,
        REQUEST_TIMEOUT         = 408,
        // 5xx Status Codes
        INTERNAL_SERVER_ERROR   = 500,
        SERVICE_UNAVALIABLE     = 503,
        // Custom Status Codes
        DEBUG_VERSION           = 1000
    };
}

#endif