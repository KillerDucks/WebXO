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

    public:
        // Constructors
        HTTP(std::string httpPath, InterceptSettings interceptSettings = InterceptSettings());
        ~HTTP() = default;

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
            CompBuffer body = GetRequestedFile(httpRequest);
            HTTPRes httpResponse = GenerateHTTPResponse(body.second, httpRequest);

            std::string sReponseHeaders = httpResponse.ReturnHeader();
            sReponseHeaders += "\r\n";

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