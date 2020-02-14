#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#include "HTTP.hpp"

#ifndef WEBX_SOCKS_H
#define WEBX_SOCKS_H

namespace WebX
{
    class Sockets
    {
    private:
        // Functions
        void ErrorHandler();

        // Data
        int port = 0;
        HTTP _Http;
        int socketID = 0;

    public:
        // Constructors
        Sockets(HTTP _Http, int _Port = 8080);
        ~Sockets();

        // Listening
        void Listen();
        void Example();

    };    
}

#endif