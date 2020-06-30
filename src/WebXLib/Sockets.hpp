#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>

// Random Support
#include <random>

// C Style Time Support
#include <ctime>

// Threading
#include <thread>

// Non Blocking Control
#include <fcntl.h>

// Errors
#include <errno.h>

// Standard Lib
#include <string>

// LibWebX
#include "HTTP.hpp"
#include "Logarithm.hpp"

#ifndef WEBX_SOCKS_H
#define WEBX_SOCKS_H

namespace WebX
{
    class Sockets
    {
    private:
        // Functions
        void ErrorHandler();

        static std::string random_string( size_t length )
        {
            auto randchar = []() -> char
            {
                const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
                const size_t max_index = (sizeof(charset) - 1);
                return charset[ rand() % max_index ];
            };
            std::string str(length,0);
            std::generate_n( str.begin(), length, randchar );
            return str;
        }

        // Internal Thread Handler
        typedef struct ThreadID
        {
            ThreadID() : friendlyName("NOT_SET"), id(random_string(5)), done(false), moveOut(false) {}
            std::string     friendlyName;
            std::string     id;
            bool            done;
            bool            moveOut;
        } THREADID;

        // Data
        int port = 0;
        HTTP _Http;
        Logarithm _Log;
        int socketID = 0;
        int iSockets = 0;

        // Options Sockets
        int option = 1;

    public:
        // Settings Structure
        typedef struct Settings
        {
            // Constructor
            Settings() : thread(false), max_threads(2) {}
            Settings(bool enThread, int noTheads)
            {
                this->thread = enThread;
                this->max_threads = noTheads;
            }
            bool    thread          = false;      // Enable Threading ? [DEFAULT] FALSE
            int     max_threads     = 2;          // Maximum Threads [DEFAULT] 2
        } SETTINGS, *PSETTINGS;

        // Public Variables
        Settings _Settings = Settings();

        // Constructors
        Sockets(HTTP _Http, int _Port = 8080, Settings const _Settings = Settings());
        ~Sockets();

        // Listening
        void    Listen();
        void    Example();
        
        // Request Handling
        int     RequestHandler(ThreadID const &tID);

        // Internal Thread Handler
        std::vector<std::pair<ThreadID, std::thread>> vThread;


        // Helper Function
        int GetThreadVector(std::string thread_id)
        {
            for (size_t i = 0; i < vThread.size(); i++)
            {
                if(vThread.at(i).first.id == thread_id)
                {
                    return i;
                }
            }
            return -1;
        }

    };    
}

#endif