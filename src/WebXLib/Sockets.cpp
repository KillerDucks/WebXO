#include "Sockets.hpp"

namespace WebX
{
    Sockets::Sockets(HTTP _Http, int _Port, Settings _Settings) : _Http(_Http), _Log("Socket"), _Settings(Settings())
    {
        // Set the port [NOTE] Can be moved the the init list
        this->port = _Port;        
        // Settings [NOTE] Can be moved the the init list
        this->_Settings = _Settings;
        // Verbose Logging
        _Log.iLog("[%z] [%q] Socket Server is Initalizing on Port [%d]\n", Logarithm::NOTICE, _Port);
        _Log.iLog("[%z] [%q] Socket Server Setting: Num of Thread [%d]\n", Logarithm::NOTICE, _Settings.max_threads);        
    }

    Sockets::~Sockets()
    {
        // Nothing to clean up for now :P
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
            // Create as many threads as the user requested
            if(this->vThread.size() != (size_t)this->_Settings.max_threads)
            {
                // Kick off into a thread to handle the client
                for(int i = 0; i < this->_Settings.max_threads; i++)
                {
                    // Create a Thread
                    ThreadID tID;

                    // Twice for good luck
                    tID.id = random_string(10);

                    // [INFO] Possibly un-needed Log
                    // _Log.iLog("[%z] [%q] Kicking off Thread ID: [%s]\n", Logarithm::CRITICAL, tID.id.c_str());

                    // Set the inital threads off
                    this->vThread.emplace_back(std::make_pair(tID, std::thread(&WebX::Sockets::RequestHandler, std::ref(*this), tID)));

                    iSockets++;
                }
            }
            else
            {
                // Reuse old threads
                for (int i = 0; i < iSockets; i++)
                {
                    // Check if the threaded function has decleared that it is finished
                    if(this->vThread.at(i).first.done)
                    {
                        // Check if the thread is joinable
                        if(this->vThread.at(i).second.joinable())
                        {
                            // Join the thread back to the main thread
                            this->vThread.at(i).second.join();
                            // Set the thread status back to `false`
                            this->vThread.at(i).first.done = false;
                            _Log.iLog("[%z] [%q] [%s] Reloading the thread\n", Logarithm::NOTICE, this->vThread.at(i).first.id.c_str());
                            // Reset the thread and spawn a new thread
                            this->vThread.at(i).second = std::move(std::thread(&WebX::Sockets::RequestHandler, std::ref(*this), this->vThread.at(i).first));
                        }
                    }
                }
                
            }
        }
        

        // Join back the threads [INFO] Unsure why to join the threads if we keep re-using them
        _Log.Log("Joining all threads", Logarithm::CRITICAL);
        for(auto & socTh : this->vThread)
        {
            // Check if the thread is joinable
            if(socTh.second.joinable())
            {
                // Join the thread back to the main thread
                socTh.second.join();
            }
        }


        // Shutdown the Listening Thread [NOTE] This code is never reached [CHANGE] Look into this
        shutdown(this->socketID, SHUT_RDWR);
        // Close the Listening Socket
        close(this->socketID);

        // This is never reached
        return;
    }

    int Sockets::RequestHandler(ThreadID const &tID)
    {
        // Variables !!
        char buffer[2048];
        HTTPReq hReq;
        HTTPRes hRes;
        std::string s_httpHeader;
        // std::vector<std::string> vBuffer;
        std::pair<char*, int> vBuffer;

        // Extract the thread id
        ThreadID &a_thread = const_cast<ThreadID&>(tID);
        std::string this_thread_id(a_thread.id);
        _Log.iLog("[%z] [%q] [%s] Now servering a client !!\n",Logarithm::NOTICE, this_thread_id.c_str());

        // Create Client Socket Struct + Allocate Memory + Clear Pointer Address
        struct sockaddr cSockAddr;
        socklen_t addr_size = sizeof(cSockAddr);
        memset((char*)&cSockAddr, 0, sizeof(cSockAddr));
        _Log.Log("Created a Client Socket", Logarithm::INFO);

        // Accept the new client and create a new socket to communicate on         
        int cSocket = accept(this->socketID, (struct sockaddr*)&cSockAddr, &addr_size);

        // Error checking on the client socket
        _Log.Log("Attempting to accept a client", Logarithm::INFO);
        if(cSocket == -1)
        {
            // There is an error
            close(this->socketID);
            // Throw a error
            _Log.Log("Could not accept a Client Socket, FATAL !!!", Logarithm::FATAL);
            // Fallback printf
            printf("Oh dear, something went wrong with accept()! %s\n", strerror(errno));
            return -1;
        }
        _Log.iLog("[%z] [%q] Connected to Client Socket using FD [%d]\n", Logarithm::INFO, cSocket);

        // Read in the data from the socket
        int cPos = read(cSocket, buffer, 2048 - 1);
        buffer[cPos] = '\0';                    
        
        // Parse the Incoming HTTP Request (ignore cast error if visible)
        hReq = this->_Http.ParseRequest(buffer);

        // Verbose Logging
        _Log.iLog("[%z] [%q] Serving Client @ [%s] with User-Agent [%s]\n",Logarithm::NOTICE, hReq.host.c_str(), hReq.user_Agent.c_str());
        _Log.iLog("[%z] [%q] Client is requesting [%s]\n",Logarithm::NOTICE, hReq.requestType.c_str());
        
        // Get the requested file and store into a local container
        vBuffer = this->_Http.GetRequestedFile(hReq);

        // Generate the HTTP Response Headers (ignore cast error if visible)
        hRes = this->_Http.GenerateHTTPResponse();    
 
        // Construct the HTTP Headers into a string to send back the client (This is done to add the \r\n so the data can follow)
        s_httpHeader += hRes.ReturnHeader();
        s_httpHeader += "\r\n";

        // Write the HTTP Header to the Client Socket
        write(cSocket, s_httpHeader.c_str(), s_httpHeader.size());

        // // Auto loop over the vector buffer and spit out the content back to the client
        // for(auto c : vBuffer)
        // {
        //     // If there is a space assume this is a null terminator, so send one to the client
        //     if(c == " ")
        //     {
        //         // Write to the Client Socket (size is hardcoded to 1)
        //         write(cSocket, &"\0", 1);                
        //     }
        //     else
        //     {
        //         // Write every other line normally to the CLient Socket [NOTE] [DEBUG] [CURRENT] Crashes here during siege load testing
        //         // write(cSocket, c.c_str(), c.size());             // [NOTE] The program will crash due to any broken pipes
        //         if(send(cSocket, c.c_str(), c.size(), MSG_NOSIGNAL) == -1)   // [CURRENT] Look into send flags, this flag (MSG_NOSIGNAL) ignores if the pipe is broken or not
        //         {
        //             // Oh no, we have an error !!!                    
        //             _Log.iLog("[%z] [%q] An Error has occurred with send(): [%s]\n",Logarithm::NOTICE, strerror(errno));
        //         }
        //     }            
        // }

        // Write every other line normally to the CLient Socket [NOTE] [DEBUG] [CURRENT] Crashes here during siege load testing
        
        if(send(cSocket, vBuffer.first, vBuffer.second, MSG_NOSIGNAL) == -1)   // [CURRENT] Look into send flags, this flag (MSG_NOSIGNAL) ignores if the pipe is broken or not
        {
            // Oh no, we have an error !!!                    
            _Log.iLog("[%z] [%q] An Error has occurred with send(): [%s]\n",Logarithm::NOTICE, strerror(errno));
        }

        // send(cSocket, std::string("\r\n").c_str(), std::string("\r\n").size(), MSG_NOSIGNAL);

        // Shutdown the Socket (Read & Write)
        shutdown(cSocket, SHUT_RDWR);
        // Close the Socket
        close(cSocket);

        // Log we are returing
        _Log.iLog("[%z] [%q] [%s] Returning thread back to the main thread !!\n", Logarithm::NOTICE, this_thread_id.c_str());    
        // Tell the master thread we are done
        this->vThread.at(GetThreadVector(a_thread.id)).first.done = true;

        // Return a 1 (i don't know why this cant be void but oh well)
        return 1;
    }
};