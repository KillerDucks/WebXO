// This class will allow the programatic interception of http requests to the server and inspect them

#include <cstdio>
#include <string>
#include <vector>
#include <string>
#include <functional>
#include <map>
#include "Logarithm.hpp"
#include "HTTPMethodTypes.hpp"
#include "HTTPHeaders.hpp"

#ifndef WEBX_INTERCEPTION_H
#define WEBX_INTERCEPTION_H

namespace WebXO
{
    // Interception Settings [TODO] Move into a separate file after testing
    typedef struct InterceptSettings{
        InterceptSettings() : isBlocking(false), method(HTTPMethodTypes::GET), callback([=](HTTPReq req) { printf ("Requested: %s\n", req.requestType.c_str()); })
        {}

        bool isBlocking;
        HTTPMethodTypes method;
        std::function<void(HTTPReq)> callback;
    } INTERCEPTIONSETTINGS;

    // Interception Class
    class Interception
    {
    private:
        // Functions
        void ErrorHandler();

        // Data
        bool isBlocking;            // Make the interception block the request

        // Logger
        Logarithm _Log;
    public:
        // De/Constructors
        Interception(InterceptSettings interceptSettings);                                      // Single Method to catch
        Interception(HTTPMethodTypes method = HTTPMethodTypes::GET, bool isBlocking = false);   // Single Method to catch
        // Interception(std::vector<HTTPMethodTypes> methods , bool isBlocking = false);           // Multiple Methods to catch
        ~Interception() = default;

        std::tuple<HTTPReq, char*> HookSync(HTTPReq info, std::function<void(HTTPReq&)> func)
        {
            // Something
            func(info);
            return std::tuple<HTTPReq, char*>(info, (char*)"-1");
        }

        void HookAsync(std::function<void()> func);

    };    
}

#endif