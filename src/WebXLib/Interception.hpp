#pragma once

// This class will allow the programmatic interception of http requests to the server and inspect them

#include <functional>

#include "Common.hpp"
#include "Logarithm.hpp"
#include "HTTPHeaders.hpp"
#include "HTTPMethodTypes.hpp"

#ifndef WEBX_INTERCEPTION_H
#define WEBX_INTERCEPTION_H

namespace WebXO
{
    // Interception Settings [TODO] Move into a separate file after testing
    struct InterceptSettings{
        InterceptSettings():method(HTTPMethodTypes::GET), 
                            callback([=](HTTPReq &req) -> CompBuffer 
                            {
                                // Make sure we are targetting the correct method [TODO]
                                std::string sHTTPMethod(req.requestType.substr(0, req.requestType.find(' ')));
                                // This is just a default interception that will do nothing
                                return CompBuffer((char*)"TEST", -2);
                            })
        {}

        bool isBlocking;
        HTTPMethodTypes method;
        std::function<CompBuffer(HTTPReq&)> callback;
    };

    // Interception Class
    class Interception
    {
    private:
        // Functions
        void ErrorHandler();

    public:
        // De/Constructors
        Interception(InterceptSettings interceptSettings)                                           // Single Method to catch
        {};                                      
        Interception(HTTPMethodTypes method = HTTPMethodTypes::GET)                                 // Single Method to catch
        {};
        // Interception(std::vector<HTTPMethodTypes> methods , bool isBlocking = false);            // Multiple Methods to catch
        ~Interception() = default;

        std::tuple<HTTPReq, CompBuffer> HookSync(HTTPReq info, std::function<CompBuffer(HTTPReq&)> func)
        {
            return std::tuple<HTTPReq, CompBuffer>(info, func(info));
        }
    };    
}

#endif