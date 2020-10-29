#pragma once

#include <ctime>

#include "Common.hpp"

#ifndef WEBX_HTTPHEADERS_H
#define WEBX_HTTPHEADERS_H

namespace WebXO
{

    typedef struct hGeneral
    {
        // string  Cache_Control;      Whats caching????
        std::string  Connection              = "Connection: close";
        std::string  Date;
        // std::string  Pragma;
        // std::string  Trailer;          
        // std::string  Transfer_Encoding      = "Transfer-Encoding: deflate"; //Currently not using any kind of encoding to speed up the transmission
        // std::string  Upgrade;           Currently we don't need to upgrade the protocols
        // std::string  Via;               We are not using Proxies or gateways in this POC
        // std::string  Warning;           Not used in this POC

        inline std::string  GetCurrentTime()
        {
            std::time_t result = std::time(nullptr);
            std::string t;
            t += "Date: ";
            t += std::asctime(std::localtime(&result));
            return t;
        }

        inline std::string  ReturnHeader()
        {
            std::string t;
            t += Connection;
            t += "\r\n";
            t += Date;
            // t += "\r\n";
            return t;
        }
    } HGENERAL, *PHGENERAL;


    typedef struct hEntity
    {
        std::string      Allow                       = "Allow: GET, POST";
        std::string      contentEncoding;                
        std::string      contentLanguage             = "Content-Language: en-GB, en-US";
        std::string      contentLength;              // [NOTE] [FUTURE] Fix the content length
        // std::string      contentLocation;         Currently not in use
        // std::string      contentMD5;              Not yet implemented in the POC
        // std::string      contentRange;            Currently not in use
        std::string      contentType                 = "Content-Type: text/html;";
        // std::string      Expires;                 Currently not in use
        // std::string      Last_Modified;           Currently not in use
        // std::string      extension_header;        Currently not in use

        void        PopulateContentLength(char* msg)
        {
            contentLength = strlen(msg) + sizeof(hEntity);
        }

        std::string      ReturnHeader()
        {
            std::string t;
            t += Allow;
            t += "\r\n";
            t += contentType;
            t += "\r\n";
            t += contentLanguage;
            t += "\r\n";
            if(!contentEncoding.empty())
            {
                t += contentEncoding;
                t += "\r\n";
            }
            t += contentLength;
            t += "\r\n";            
            return t;
        }
    } HENTITY, *PHENTITY;

    typedef struct HTTPReq
    {
        std::string   requestType;
        std::string   host;
        std::string   connection;
        int      dnt;
        int      upgrade_insecure_requests;
        std::string   user_Agent;
        std::string   sec_Fetch_User;
        std::string   accept;
        std::string   sec_Fetch_Site;
        std::string   sec_Fetch_Mode;
        std::string   accept_Encoding;
        std::string   accept_Languages;

        std::string file()
        {
            if(requestType.size() != 0)
            {
                return requestType.substr(requestType.find(' ') + 1, requestType.size());
            }
            return std::string("-1");
        }
        std::string method()
        {
            if(requestType.size() != 0)
            {
                return requestType.substr(0, requestType.find(' '));
            }
            return std::string("-1");
        }
    } HTTPREQ, *PHTTPREQ;


    typedef struct HTTPRes
        {
            // Status Header
            std::string      httpVersion             = HTTP_VERSION;
            int              statusCode;
            std::string      reasonPhrase;
            // General Header
            hGeneral    httpGeneralHeader;
            // Response Header
            std::string      acceptRanges            = "Accept-Ranges: none";
            std::string      age                     = "Age : 0";
            // std::string      eTag;                We currently dont need to use the entity tag
            // std::string      location;            We currently dont need to use location
            // std::string      proxyAuthenticate;   We currently dont need to use Proxy Authentication
            // std::string      retryAfter;          We currently dont need to use Retry-After
            std::string      server                  = SERVER_STRING;
            // string      vary;                Not needed as we are not using cache-control
            // string      wwwAuthenticate;     Not currently implementing Status Code 401 so this is not needed
            hEntity     httpEntityHeader;
            // Helper Functions
            std::string      GenerateStatusLine()
            {
                std::string t;
                t += httpVersion;
                t +=  " ";
                t += std::to_string(statusCode);
                t += " ";
                t += reasonPhrase;
                t += "\r\n";
                return t;
            }

            std::string      ReturnHeader()
            {
                std::string t;
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
}

#endif