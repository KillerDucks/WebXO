#pragma once

#include <regex>
#include <ctime>

#include "Common.hpp"
#include "HTTPHeaders.hpp"
#include "HTTPStatusCodes.hpp"
#include "Directory.hpp"
#include "Logarithm.hpp"
#include "Interception.hpp"
#include "Compression.hpp"
#include "VirtualHosts.hpp"
#include "FolderView.hpp"

#include "IO.hpp"

// OneAgent SDK
#include <onesdk/onesdk.h>

#ifndef WEBXO_HTTP_H
#define WEBXO_HTTP_H

constexpr unsigned int hash(const char *s, int off = 0) {                        
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];                           
}                                                                                


using std::string;
using std::vector;
using std::fstream;

namespace WebXO
{
    class HTTP
    {
    private:
        // Functions
        void ErrorHandler();
        bool AcceptDeflate(std::string accept_encodings);

        // Directory Services
        Directory iDirectory;

        // VirtualHost Services
        VirtualHosts vHosts;

        // HTTP Status Codes
        WebXO::HTTPStatusCodes httpCode;

        // HTTP Methods
        WebXO::HTTPMethodTypes httpMethod;

        // Interception
        bool isIntercept;
        InterceptSettings _interceptionSettings = InterceptSettings();
        Interception _interception;

        // Content Type
        enum class  MimeType
        {
            HTML,
            JS,
            CSS,

            // [CHANGE] This is too generic
            IMAGE,

            VIDEO
        } MIMETYPE;

        // Helper Functions
        enum MimeType   GetMIMEType(std::string filePath);

        // OneAgent WebApp Handle
        onesdk_webapplicationinfo_handle_t web_application_info_handle = ONESDK_INVALID_HANDLE;

    public:
        // Constructors
        HTTP(std::string httpPath, InterceptSettings interceptSettings = InterceptSettings());
        ~HTTP() {
            // Cleanup the OneAgent Handle
            onesdk_webapplicationinfo_delete(this->web_application_info_handle);
            this->web_application_info_handle = ONESDK_INVALID_HANDLE;
        }

        // Parse HTTP Requests
        HTTPReq ParseRequest(char* request);

        // Handle page Requests
        CompBuffer GetRequestedFile(HTTPReq hReq);

        // Generate a HTTP Response Header
        HTTPRes GenerateHTTPResponse(int contentLength, HTTPReq hReq);

        // Responder
        std::pair<CompBuffer, std::string> Response(char* buffer)
        {
            HTTPReq httpRequest = ParseRequest(buffer);

             /* create tracer */
            onesdk_tracer_handle_t const tracer = onesdk_incomingwebrequesttracer_create(
                web_application_info_handle,
                onesdk_asciistr(httpRequest.requestType.c_str()),
                onesdk_asciistr(httpRequest.method().c_str()));

            /* add information about the incoming request */
            onesdk_incomingwebrequesttracer_set_remote_address(tracer, onesdk_asciistr(httpRequest.host.c_str()));
            onesdk_incomingwebrequesttracer_add_request_header(tracer,
                onesdk_asciistr("Connection"), onesdk_asciistr(httpRequest.connection.c_str()));
            onesdk_incomingwebrequesttracer_add_request_header(tracer,
                onesdk_asciistr("User-Agent"), onesdk_asciistr(httpRequest.user_Agent.c_str()));
            onesdk_incomingwebrequesttracer_add_request_header(tracer,
                onesdk_asciistr("Accept-Encoding"), onesdk_asciistr(httpRequest.accept_Encoding.c_str()));

            /* start tracer */
            onesdk_tracer_start(tracer);

            CompBuffer body = GetRequestedFile(httpRequest);
            HTTPRes httpResponse = GenerateHTTPResponse(body.second, httpRequest);

            std::string sReponseHeaders = httpResponse.ReturnHeader();
            sReponseHeaders += "\r\n";

            /* add information about the response */
            onesdk_incomingwebrequesttracer_add_response_header(tracer,
                onesdk_asciistr("Content-Length"), onesdk_asciistr(httpResponse.httpEntityHeader.contentLength.c_str()));
            onesdk_incomingwebrequesttracer_add_response_header(tracer,
                onesdk_asciistr("Content-Encoding"), onesdk_asciistr(httpResponse.httpEntityHeader.contentEncoding.c_str()));
            onesdk_incomingwebrequesttracer_add_response_header(tracer,
                onesdk_asciistr("Content-Type"), onesdk_asciistr(httpResponse.httpEntityHeader.contentType.c_str()));
            onesdk_incomingwebrequesttracer_add_response_header(tracer,
                onesdk_asciistr("Server"), onesdk_asciistr(httpResponse.server.c_str()));
            onesdk_incomingwebrequesttracer_set_status_code(tracer, httpResponse.statusCode);

            /* set error information */
            if (httpCode != HTTPStatusCodes::OK) {
                switch (httpCode)
                {
                case HTTPStatusCodes::NOT_FOUND:
                    onesdk_tracer_error(tracer, onesdk_asciistr("HTTP Error"), onesdk_asciistr("[404] Page not found"));
                    break;
                
                case HTTPStatusCodes::NO_CONTENT:
                    onesdk_tracer_error(tracer, onesdk_asciistr("HTTP Error"), onesdk_asciistr("[204] No Content"));
                    break;
                }
            }                

            /* end & release tracer */
            onesdk_tracer_end(tracer);

            if(this->httpMethod == HTTPMethodTypes::HEAD)
            {
                // Dont send the body
                return {CompBuffer((char*)"NULL", -1), sReponseHeaders};
            }

            return {body, sReponseHeaders};
        };
    };    
}

#endif