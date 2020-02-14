#include "Sockets.hpp"

namespace WebX
{
    Sockets::Sockets(HTTP _Http, int _Port)
    {
        this->port = _Port;
        this->_Http = _Http;
    }

    Sockets::~Sockets()
    {
    }


    void Sockets::Example()
    {
        printf("Hello World\n");
    }

    void Sockets::Listen()
    {
        // Create a new socket
        this->socketID = socket(AF_INET, SOCK_STREAM, 0);

        // Create the Socket Configuration
        struct sockaddr_in serverAddr;
        memset((char*)&serverAddr, 0, sizeof(serverAddr));

        // Socket Address Configuration
        serverAddr.sin_port = htons(this->port);
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        // Bind the new Socket to the Network Interface
        bind(this->socketID, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

        // Listen for clients
        listen(this->socketID, 1);

        // Create Client Socket Struct
        struct sockaddr cSockAddr;
        socklen_t addr_size = sizeof(cSockAddr);
        memset((char*)&cSockAddr, 0, sizeof(cSockAddr));

        // Accept the new client and create a new socket to communicate on
        int cSocket = accept(this->socketID, (struct sockaddr*)&cSockAddr, &addr_size);
        if(cSocket == -1)
        {
            // There is an error
            close(this->socketID);
            // Throw a error
            return;
        }

        // Read in the data from the socket
        char buffer[2048];
        HTTP::HTTPReq hReq;
        HTTP::HTTPRes hRes;
        string x, y;

        int cPos = read(cSocket, buffer, 2048 - 1);
        buffer[cPos] = '\0';                    
        
        hReq = this->_Http.ParseRequest(buffer);

        printf("Client is requesting [%s]\n", hReq.requestType.c_str());

        x = this->_Http.GetRequestedFile(hReq);

        hRes = this->_Http.GenerateHTTPResponse((char*)x.c_str());

        y = hRes.ReturnHeader();
        y += "\r\n";
        y += x;

        write(cSocket, y.c_str(), strlen(y.c_str()));

        close(cSocket);

        close(this->socketID);

        return;
    }
};