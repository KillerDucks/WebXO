#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

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
        int socketID = 0;

    public:
        // Constructors
        Sockets(int _Port = 8080);
        ~Sockets();

        // Listening
        void Listen();
        void Example();

    };    
}

#endif