#include <stdio.h>
#include <string.h>
#include <vector>
#include <regex>
#include <string>

#ifndef WEBX_HTTP_H
#define WEBX_HTTP_H

using std::string;
using std::vector;

namespace WebX
{
    class HTTP
    {
    private:
        // Functions
        void ErrorHandler();

        // Data
    public:
        // Structs
        typedef struct HTTPReq
        {
            char*   requestType;
            char*   host;
            char*   connection;
            int     dnt;
            int     upgrade_insecure_requests;
            char*   user_Agent;
            char*   sec_Fetch_User;
            char*   accept;
            char*   sec_Fetch_Site;
            char*   sec_Fetch_Mode;
            char*   accept_Encoding;
            char*   accept_Languages;
        } HTTPREQ, *PHTTPREQ;

        // Constructors
        HTTP();
        ~HTTP();

        // Parse HTTP Requests
        HTTPReq ParseRequest(char* request);

    };    
}

#endif