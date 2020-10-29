#include <climits>
#include <cstdarg>
#include <cstring>
#include <algorithm>

// [DEBUG]
#include <iostream>

#include <stdlib.h>

#ifndef LOGARITHM_H
#define LOGARITHM_H

namespace WebXO
{
    class Logarithm
    {
    private:
        std::string _program_name = "Logarithm";
        // char       _pName[4] = {"LOG"};
        int         _default_log_Level;

        // Interception
        static bool print(const char* data, size_t length) {
            const unsigned char* bytes = (const unsigned char*) data;
            for (size_t i = 0; i < length; i++)
            {
                if(printf("%c", bytes[i]) == -1)
                {
                    return false;
                }
                
            }            
            return true;
        }

    public:
        Logarithm(std::string progName = (std::string)"Logarithm", int defaultLevel = 0);
        ~Logarithm();

        // Log Levels
        enum LogLevels
        {
            DEFAULT     = -1,
            // General Messages
            INFO        = 0,
            NOTICE      = 1,
            // Alerts
            WARNING     = 2,
            CRITICAL    = 3,
            FATAL       = 4
        };

        // Log functions
        void        Log(std::string message, LogLevels ll);
        int         iLog(const char* format, ...);
    };
};

#endif // LOGARITHM_H