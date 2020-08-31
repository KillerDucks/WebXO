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

        // Logging
        Logarithm _Log;

        // Directory Services
        Directory iDirectory;

        // VirtualHost Services
        VirtualHosts vHosts;

        // HTTP Status Codes
        WebXO::HTTPStatusCodes httpCode;

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
            IMAGE 
        } MIMETYPE;

        // Helper Functions
        enum MimeType   GetMIMEType(std::string filePath);

    public:
        // Constructors
        HTTP(std::string httpPath, InterceptSettings interceptSettings = InterceptSettings());
        ~HTTP();

        // Parse HTTP Requests
        HTTPReq ParseRequest(char* request);

        // Handle page Requests
        std::pair<char*, int>   GetRequestedFile(HTTPReq hReq);

        // Generate a HTTP Response Header
        HTTPRes GenerateHTTPResponse(int contentLength, HTTPReq hReq);
    };    
}

#endif