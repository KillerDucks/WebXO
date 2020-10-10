#include "Common.hpp"
#include "HTTPHeaders.hpp"
#include "HTTPStatusCodes.hpp"
#include "Logarithm.hpp"

#ifndef WEBX_VIRTUALHOSTS_H
#define WEBX_VIRTUALHOSTS_H

namespace WebXO
{
    class VirtualHosts
    {
    private:
        // Functions
        std::map<std::string, std::string> Parse(std::string filename);
        // Logging
        Logarithm _Log;

        // Variables
        std::map<std::string, std::string> vHosts;

    public:
        // Constructors
        VirtualHosts(std::string filename);
        ~VirtualHosts() = default;

        std::string Query(std::string host);
    };    
}

#endif