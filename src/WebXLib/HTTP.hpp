#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <ctime>

#include "HTTPStatusCodes.hpp"
#include "Directory.hpp"
#include "Logarithm.hpp"

#ifndef WEBX_HTTP_H
#define WEBX_HTTP_H

constexpr char* HTTP_VERSION    = (char*)"HTTP/1.1";
constexpr char* SERVER_STRING   = (char*)"Server: WebXO/0.1";

constexpr unsigned int hash(const char *s, int off = 0) {                        
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];                           
}                                                                                


using std::string;
using std::vector;
using std::fstream;

namespace WebX
{
    class HTTP
    {
    private:
        // Functions
        void ErrorHandler();

        // Logging
        Logarithm _Log;

        // Directory Services
        Directory iDirectory;

        WebX::HTTPStatusCodes httpCode;

        // Data
        typedef struct hGeneral
        {
            // string  Cache_Control;      Whats caching????
            string  Connection              = "Connection: close";
            string  Date;
            // string  Pragma;
            // string  Trailer;          
            // string  Transfer_Encoding; Currently not using any kind of encoding to speed up the transmission
            // string  Upgrade;           Currently we don't need to upgrade the protocols
            // string  Via;               We are not using Proxies or gateways in this POC
            // string  Warning;           Not used in this POC

            inline string  GetCurrentTime()
            {
                std::time_t result = std::time(nullptr);
                string t;
                t += "Date: ";
                t += std::asctime(std::localtime(&result));
                return t;
            }

            inline string  ReturnHeader()
            {
                string t;
                t += Connection;
                t += "\r\n";
                t += Date;
                // t += "\r\n";
                return t;
            }
        } HGENERAL, *PHGENERAL;

        typedef struct hEntity
        {
            string      Allow                       = "Allow: GET, POST";
            // string      contentEncoding;         Currently not in use
            string      contentLanguage             = "Content-Language: en-GB, en-US";
            string      contentLength;
            // string      contentLocation;         Currently not in use
            // string      contentMD5;              Not yet implemented in the POC
            // string      contentRange;            Currently not in use
            string      contentType                 = "Content-Type: text/html;";
            // string      Expires;                 Currently not in use
            // string      Last_Modified;           Currently not in use
            // string      extension_header;        Currently not in use

            void        PopulateContentLength(char* msg)
            {
                contentLength = strlen(msg) + sizeof(hEntity);
            }

            string      ReturnHeader()
            {
                string t;
                t += Allow;
                t += "\r\n";
                t += contentLanguage;
                t += "\r\n";
                t += contentLength;
                t += "\r\n";
                t += contentType;
                t += "\r\n";
                return t;
            }
        } HENTITY, *PHENTITY;

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
        // Structs
        typedef struct HTTPReq
        {
            string   requestType;
            string   host;
            string   connection;
            int      dnt;
            int      upgrade_insecure_requests;
            string   user_Agent;
            string   sec_Fetch_User;
            string   accept;
            string   sec_Fetch_Site;
            string   sec_Fetch_Mode;
            string   accept_Encoding;
            string   accept_Languages;
        } HTTPREQ, *PHTTPREQ;

        typedef struct HTTPRes
        {
            // Status Header
            string      httpVersion             = HTTP_VERSION;
            int         statusCode;
            string      reasonPhrase;
            // General Header
            hGeneral    httpGeneralHeader;
            // Response Header
            string      acceptRanges            = "Accept-Ranges: none";
            string      age                     = "Age : 0";
            // string      eTag;                We currently dont need to use the entity tag
            // string      location;            We currently dont need to use location
            // string      proxyAuthenticate;   We currently dont need to use Proxy Authentication
            // string      retryAfter;          We currently dont need to use Retry-After
            string      server                  = SERVER_STRING;
            // string      vary;                Not needed as we are not using cache-control
            // string      wwwAuthenticate;     Not currently implementing Status Code 401 so this is not needed
            hEntity     httpEntityHeader;
            // Helper Functions
            string      GenerateStatusLine()
            {
                string t;
                t += httpVersion;
                t +=  " ";
                t += std::to_string(statusCode);
                t += " ";
                t += reasonPhrase;
                t += "\r\n";
                return t;
            }

            string      ReturnHeader()
            {
                string t;
                t += GenerateStatusLine();
                // t += "\r\n";
                t += httpGeneralHeader.ReturnHeader();
                // t += "\r\n";
                t += acceptRanges;
                t += "\r\n";
                t += age;
                t += "\r\n";
                t += server;
                t += "\r\n";
                t += httpEntityHeader.ReturnHeader();
                // t += "\r\n";
                return t;
            }
        } HTTPRES, *PHTTPRES;

        // Constructors
        HTTP(std::string httpPath);
        ~HTTP();

        // Parse HTTP Requests
        HTTPReq ParseRequest(char* request);

        // Handle page Requests
        std::vector<std::string>   GetRequestedFile(HTTPReq hReq);

        // Generate a HTTP Response Header
        HTTPRes GenerateHTTPResponse(std::vector<char*> message);

    };    
}

#endif