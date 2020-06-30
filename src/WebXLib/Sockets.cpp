#include "Sockets.hpp"

namespace WebX
{
    Sockets::Sockets(HTTP _Http, int _Port, Settings _Settings) : _Http(_Http), _Log("Socket"), _Settings(Settings())
    {
        this->port = _Port;
        // this->_Http = _Http;
        
        // Settings
        this->_Settings = _Settings;
        _Log.iLog("[%z] [%q] Socket Server is Initalizing on Port [%d]\n", Logarithm::NOTICE, _Port);
        _Log.iLog("[%z] [%q] Socket Server Setting: Num of Thread [%d]\n", Logarithm::NOTICE, _Settings.max_threads);

        
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
        this->socketID = 0;
        // Create a new socket        
        if((this->socketID = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            _Log.Log("FATAL Error: Could not create the Socket", Logarithm::FATAL);    
            // Printf Extra Error Tracing
            printf("Oh dear, something went wrong with socket()! %s\n", strerror(errno));
            return;
        }
        _Log.iLog("[%z] [%q] Created a new Socket using FD [%d]\n", Logarithm::INFO, this->socketID);

        setsockopt(this->socketID, SOL_SOCKET, SO_REUSEADDR, &this->option, sizeof(this->option));

        // Create the Socket Configuration
        struct sockaddr_in serverAddr;
        memset((char*)&serverAddr, 0, sizeof(serverAddr));

        // Socket Address Configuration
        serverAddr.sin_port = htons(this->port);
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        // Bind the new Socket to the Network Interface        
        if(bind(this->socketID, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
        {
            _Log.Log("FATAL Error: Could not bind the Socket", Logarithm::FATAL);    
            // Printf Extra Error Tracing
            printf("Oh dear, something went wrong with bind()! %s\n", strerror(errno));
            return;
        }
        _Log.iLog("[%z] [%q] Bound the Socket Server to the Port [%d]\n", Logarithm::INFO, this->port);

        // Listen for clients        
        if(listen(this->socketID, 1) == -1)
        {
            _Log.Log("FATAL Error: Could not listen for clients", Logarithm::FATAL);    
            // Printf Extra Error Tracing
            printf("Oh dear, something went wrong with listen()! %s\n", strerror(errno));
            return;
        }
        _Log.Log("Listening for Clients", Logarithm::INFO);

        while (true)
        {            
            if(this->vThread.size() != (size_t)this->_Settings.max_threads)
            {
                // Kick off into a thread to handle the client
                for(int i = 0; i < this->_Settings.max_threads; i++)
                {
                    // Create a Thread
                    ThreadID tID;

                    // Twice for good luck
                    tID.id = random_string(10);

                    _Log.iLog("[%z] [%q] Kicking off Thread ID: [%s]\n", Logarithm::CRITICAL, tID.id.c_str());

                    this->vThread.emplace_back(std::make_pair(tID, std::thread(&WebX::Sockets::RequestHandler, std::ref(*this), tID)));

                    iSockets++;
                }
            }
            else
            {
                // Reuse old threads
                for (int i = 0; i < iSockets; i++)
                {
                    if(this->vThread.at(i).first.done)
                    {
                        if(this->vThread.at(i).second.joinable())
                        {
                            this->vThread.at(i).second.join();
                            this->vThread.at(i).first.done = false;
                            _Log.iLog("[%z] [%q] [%s] Reloading the thread\n", Logarithm::NOTICE, this->vThread.at(i).first.id.c_str());
                            this->vThread.at(i).second = std::move(std::thread(&WebX::Sockets::RequestHandler, std::ref(*this), this->vThread.at(i).first));
                        }
                    }
                }
                
            }
        }
        

        // Join back the threads
        _Log.Log("Joining all threads", Logarithm::CRITICAL);
        for(auto & socTh : this->vThread)
        {
            if(socTh.second.joinable())
            {
                socTh.second.join();
            }
        }

        shutdown(this->socketID, SHUT_RDWR);
        close(this->socketID);

        return;
    }

    int Sockets::RequestHandler(ThreadID const &tID)
    {
        // Check if we are in a thread
        ThreadID &a_thread = const_cast<ThreadID&>(tID);
        std::string this_thread_id(a_thread.id);
        _Log.iLog("[%z] [%q] [%s] Now servering a client !!\n",Logarithm::NOTICE, this_thread_id.c_str());

        // Create Client Socket Struct
        struct sockaddr cSockAddr;
        socklen_t addr_size = sizeof(cSockAddr);
        memset((char*)&cSockAddr, 0, sizeof(cSockAddr));
        _Log.Log("Created a Client Socket", Logarithm::INFO);

        // Accept the new client and create a new socket to communicate on         
        int cSocket = accept(this->socketID, (struct sockaddr*)&cSockAddr, &addr_size);

        // // Convert the Socket into a Non-blocking one
        // fcntl(this->socketID, F_SETFL, O_NONBLOCK);
        
        _Log.Log("Attempting to accept a client", Logarithm::INFO);
        if(cSocket == -1)
        {
            // There is an error
            close(this->socketID);
            // Throw a error
            _Log.Log("Could not accept a Client Socket, FATAL !!!", Logarithm::FATAL);
            printf("Oh dear, something went wrong with accept()! %s\n", strerror(errno));
            return -1;
        }
        _Log.iLog("[%z] [%q] Connected to Client Socket using FD [%d]\n", Logarithm::INFO, cSocket);

        // Read in the data from the socket
        char buffer[2048];
        HTTP::HTTPReq hReq;
        HTTP::HTTPRes hRes;
        string y;
        std::vector<std::string> vBuffer;

        int cPos = read(cSocket, buffer, 2048 - 1);
        buffer[cPos] = '\0';                    
        
        hReq = this->_Http.ParseRequest(buffer);

        _Log.iLog("[%z] [%q] Serving Client @ [%s] with User-Agent [%s]\n",Logarithm::NOTICE, hReq.host.c_str(), hReq.user_Agent.c_str());
        _Log.iLog("[%z] [%q] Client is requesting [%s]\n",Logarithm::NOTICE, hReq.requestType.c_str());

        vBuffer = this->_Http.GetRequestedFile(hReq);

        _Log.iLog("[%z] [%q] vBuffer Size [%d]\n",Logarithm::NOTICE, vBuffer.size());

        hRes = this->_Http.GenerateHTTPResponse((std::vector<char*>){});    
 
        char pFixer = '\0';

        y = hRes.ReturnHeader();
        y += "\r\n";
        write(cSocket, y.c_str(), strlen(y.c_str()));

        for(auto c : vBuffer)
        {
            if(c == " ")
            {
                write(cSocket, &pFixer, 1);
            }
            else
            {
                write(cSocket, c.c_str(), c.size());
            }
            
        }

        shutdown(cSocket, SHUT_RDWR);
        close(cSocket);

        // Log we are returing
        _Log.iLog("[%z] [%q] [%s] Returning thread back to the main thread !!\n", Logarithm::NOTICE, this_thread_id.c_str());    
        // Tell the master thread we are done
        this->vThread.at(GetThreadVector(a_thread.id)).first.done = true;

        return 1;
    }
};