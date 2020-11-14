#include "Sockets.hpp"

namespace WebXO
{
    Sockets::Sockets(HTTP _Http, int _Port, Settings _Settings) : _Http(_Http), _Settings(Settings())
    {
        // Set the port [NOTE] Can be moved the the init list
        this->port = _Port;        
        // Settings [NOTE] Can be moved the the init list
        this->_Settings = _Settings;
        // Verbose Logging
        Logarithm::Log(std::string("Socket"), "[%z] [%q] Socket Server is Initalizing on Port [%d]\n", Logarithm::NOTICE, _Port);
        Logarithm::Log(std::string("Socket"), "[%z] [%q] Socket Server Setting: Num of Thread [%d]\n", Logarithm::NOTICE, _Settings.max_threads);        
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
            Logarithm::Log(std::string("Socket"), "[%z] [%q] FATAL Error: Could not create the Socket\n", Logarithm::FATAL);
            // Printf Extra Error Tracing
            printf("Oh dear, something went wrong with socket()! %s\n", strerror(errno));
            return;
        }
        Logarithm::Log(std::string("Socket"), "[%z] [%q] Created a new Socket using FD [%d]\n", Logarithm::INFO, this->socketID);

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
            Logarithm::Log(std::string("Socket"), "FATAL Error: Could not bind the Socket", Logarithm::FATAL);    
            // Printf Extra Error Tracing
            printf("Oh dear, something went wrong with bind()! %s\n", strerror(errno));
            return;
        }
        Logarithm::Log(std::string("Socket"), "[%z] [%q] Bound the Socket Server to the Port [%d]\n", Logarithm::INFO, this->port);

        // Listen for clients        
        if(listen(this->socketID, 1) == -1)
        {
            Logarithm::Log(std::string("Socket"), "[%z] [%q] FATAL Error: Could not listen for clients\n", Logarithm::FATAL);
            // Printf Extra Error Tracing
            printf("Oh dear, something went wrong with listen()! %s\n", strerror(errno));
            return;
        }
        Logarithm::Log(std::string("Socket"), "[%z] [%q] Fistening for Clients\n", Logarithm::INFO);

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
                    this->vThread.emplace_back(std::make_pair(tID, std::thread(&WebXO::Sockets::RequestHandler, this, tID)));

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
                            Logarithm::Log(std::string("Socket"), "[%z] [%q] [%s] Reloading the thread\n", Logarithm::NOTICE, this->vThread.at(i).first.id.c_str());
                            // Reset the thread and spawn a new thread
                            this->vThread.at(i).second = std::move(std::thread(&WebXO::Sockets::RequestHandler, this, this->vThread.at(i).first));
                        }
                    }
                }
                
            }
        }
        

        // Join back the threads [INFO] Unsure why to join the threads if we keep re-using them
        Logarithm::Log(std::string("Socket"), "[%z] [%q] FJoining all threads\n", Logarithm::CRITICAL);
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

    void Sockets::RequestHandler(ThreadID const &tID)
    {
        // Variables !!
        char buffer[REQUEST_BUFFER];
        CompBuffer vBuffer;

        // Extract the thread id
        ThreadID &a_thread = const_cast<ThreadID&>(tID);
        std::string this_thread_id(a_thread.id);
        Logarithm::Log(std::string("Socket"), "[%z] [%q] [%s] Now servering a client !!\n",Logarithm::NOTICE, this_thread_id.c_str());

        // Create Client Socket Struct + Allocate Memory + Clear Pointer Address
        struct sockaddr cSockAddr;
        socklen_t addr_size = sizeof(cSockAddr);
        memset((char*)&cSockAddr, 0, sizeof(cSockAddr));
        Logarithm::Log(std::string("Socket"), "[%z] [%q] Created a Client Socket\n", Logarithm::NOTICE);

        // Accept the new client and create a new socket to communicate on         
        int cSocket = accept(this->socketID, (struct sockaddr*)&cSockAddr, &addr_size);

        // Error checking on the client socket
        Logarithm::Log(std::string("Socket"), "[%z] [%q] Attempting to accept a client\n", Logarithm::NOTICE);
        if(cSocket == -1)
        {
            // There is an error
            close(this->socketID);
            // Throw a error
            Logarithm::Log(std::string("Socket"), "[%z] [%q] Could not accept a Client Socket, FATAL !!!\n", Logarithm::NOTICE);
            // Fallback printf
            printf("Oh dear, something went wrong with accept()! %s\n", strerror(errno));
            return;
        }
        Logarithm::Log(std::string("Socket"), "[%z] [%q] Connected to Client Socket using FD [%d]\n", Logarithm::INFO, cSocket);

        // Read in the data from the socket
        int cPos = read(cSocket, buffer, 2048 - 1);
        buffer[cPos] = '\0';                    

        // Generate the Reponse to the Request
        std::pair<CompBuffer, std::string> response = this->_Http.Response(buffer);

        // printf("File Size: [~%dB]\n", vBuffer.second); // [DEBUG] Print

        // Write the HTTP Header to the Client Socket [PROBLEM] [HALT] The end of the HTTP Header get appended to the next transmission
        if(!response.second.empty())
        {
            cPos = send(cSocket, response.second.c_str(), response.second.size(), MSG_NOSIGNAL);
        }
        
        // printf("!!\n\n%s\n\n!!", s_httpHeader.c_str());

        // Check if the buffer needs to be sent
        if(response.first.second != -1)
        {
            if(send(cSocket, response.first.first, response.first.second, MSG_NOSIGNAL) == -1)   // [CURRENT] Look into send flags, this flag (MSG_NOSIGNAL) ignores if the pipe is broken or not
            {
                // Oh no, we have an error !!!                    
                Logarithm::Log(std::string("Socket"), "[%z] [%q] An Error has occurred with send(): [%s]\n",Logarithm::NOTICE, strerror(errno));
            }
        }
        else
        {
            printf("\n\nFATAL ERROR: Buffer Returned as NULL -1\n\n");
            exit(-1);
        }
        

        // Shutdown the Socket (Read & Write)
        shutdown(cSocket, SHUT_RDWR);
        // Close the Socket
        close(cSocket);

        // Log we are returing
        Logarithm::Log(std::string("Socket"), "[%z] [%q] [%s] Returning thread back to the main thread !!\n", Logarithm::NOTICE, this_thread_id.c_str());    
        // Tell the master thread we are done
        this->vThread.at(GetThreadVector(a_thread.id)).first.done = true;
    }
};