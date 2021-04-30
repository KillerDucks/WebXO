#pragma once

#include "Common.hpp"

#include <cstdarg>
#include <climits>
#include <stdlib.h>

#ifndef LOGARITHM_H
#define LOGARITHM_H

namespace WebXO
{
    namespace Logarithm
    {
        // Interception
        inline bool print(const char* data, size_t length) {
            // const unsigned char* bytes = (const unsigned char*) data;
            // for (size_t i = 0; i < length; i++)
            // {
            //     if(printf("%c", bytes[i]) == -1)
            //     {
            //         return false;
            //     }
                
            // }            
            return true;
        }

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
        int         Log(std::string _program_name, const char* format, ...);       
    };
        
};

#endif // LOGARITHM_H