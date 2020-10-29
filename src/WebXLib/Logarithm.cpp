#include "Logarithm.hpp"

namespace WebXO
{
    Logarithm::Logarithm(std::string progName, int defaultLevel) : _program_name("Logarithm")
    {
        // Check if the Program name has been filled correctly
        if(progName.empty())
        {
            this->_program_name = (std::string)"Logarithm";
        }
        else
        {
            // Set the service name if set
            this->_program_name = progName;
        }
        
        // Set the default log level
        this->_default_log_Level = defaultLevel; // [TODO] Do some sort of error checking ???

        this->print(std::string("\0").c_str(), std::string("\0").size());
    }

    Logarithm::~Logarithm()
    {
        // Nothing to clean up...
    }

    // Log function
    void Logarithm::Log(std::string message, LogLevels ll)
    {
        // std::string strFactory;

        // if(ll == -1)
        // {
        //     // use the default log level set
        //     ll = (LogLevels)this->_default_log_Level;
        // }

        // // [TODO] There is a better way of doing this
        // switch (ll)
        // {
        // case INFO:
        //     strFactory = "\u001b[32mINFO\u001b[0m";
        //     break;
        // case NOTICE:
        //     strFactory = "\u001b[35mNOTICE\u001b[0m";
        //     break;
        // case WARNING:
        //     strFactory = "\u001b[36mWARNING\u001b[0m";
        //     break;
        // case CRITICAL:
        //     strFactory = "\u001b[33mCRITICAL\u001b[0m";
        //     break;
        // case FATAL:
        //     strFactory = "\u001b[31mFATAL\u001b[0m";
        //     break;

        // // Default Print case (-1)
        // default:
        //     break;
        // }

        // // Use a printf function to visually log our message [TODO] Change to the internal printf finction `iLog()`
        // printf("[%s] [%s] %s\n",this->_program_name.c_str(), strFactory.c_str(), message.c_str());
    }

    // [TODO] Possible future use
    // static bool sPrint(std::string str, size_t l)
    // {
    //     for (size_t i = 0; i < l; i++)
    //     {
    //         if(printf("%c", str.at(i)) == -1)
    //         {
    //             return false;
    //         }            
    //     }
    //     return true;      
    // }

    int Logarithm::iLog(const char* format, ...) {
        va_list parameters;
        va_start(parameters, format);

        // int written = 0;

        // while (*format != '\0') {
        //     size_t maxrem = INT_MAX - written;

        //     if (format[0] != '%' || format[1] == '%') {
        //         if (format[0] == '%')
        //             format++;
        //         size_t amount = 1;
        //         while (format[amount] && format[amount] != '%')
        //             amount++;
        //         if (maxrem < amount) {
        //             // TODO: Set errno to EOVERFLOW.
        //             return -1;
        //         }
        //         if (!print(format, amount))
        //             return -1;
        //         format += amount;
        //         written += amount;
        //         continue;
        //     }

        //     const char* format_begun_at = format++;
        //     char* buffer;

        //     size_t len;

        //     switch (*format)
        //     {
        //         // Characters
        //         case 'c':
        //             {
        //                 format++;
        //                 char c = (char) va_arg(parameters, int /* char promotes to int */);
        //                 if (!maxrem) {
        //                     // TODO: Set errno to EOVERFLOW.
        //                     return -1;
        //                 }
        //                 if (!print(&c, sizeof(c)))
        //                     return -1;
        //                 written++;
        //                 break;
        //             }

        //         // Char Pointer Strings			
        //         case 's':
        //             {
        //                 format++;
        //                 const char* str = va_arg(parameters, const char*);
        //                 len = strlen(str);
        //                 if (maxrem < len) {
        //                     // TODO: Set errno to EOVERFLOW.
        //                     return -1;
        //                 }
        //                 if (!print(str, len))
        //                     return -1;
        //                 written += len;
        //                 break;
        //             }

        //         case 'z':
        //             {
        //                 format++;
        //                 if (maxrem < this->_program_name.length()) {
        //                     // TODO: Set errno to EOVERFLOW.
        //                     return -1;
        //                 }
                        
        //                 if (!print(this->_program_name.c_str(), this->_program_name.size()))
        //                     return -1;
        //                 written += len;
        //                 break;
        //             }

        //         // Log Level Tags			
        //         case 'q':
        //             {
        //                 format++;
        //                 int loglvl = va_arg(parameters, int);
        //                 std::string strFactory = "NOTICE";

        //                 LogLevels ll = (LogLevels)loglvl;

        //                 len = sizeof(loglvl);
        //                 if (maxrem < len) {
        //                     return -1;
        //                 }

        //                 if(ll == -1)
        //                 {
        //                     // use the default log level set
        //                     ll = (LogLevels)this->_default_log_Level;
        //                 }

        //                 // [TODO] There is a better way of doing this
        //                 switch (ll)
        //                 {
        //                 case INFO:
        //                     strFactory = "\u001b[32mINFO\u001b[0m";
        //                     break;
        //                 case NOTICE:
        //                     strFactory = "\u001b[35mNOTICE\u001b[0m";
        //                     break;
        //                 case WARNING:
        //                     strFactory = "\u001b[36mWARNING\u001b[0m";
        //                     break;
        //                 case CRITICAL:
        //                     strFactory = "\u001b[33mCRITICAL\u001b[0m";
        //                     break;
        //                 case FATAL:
        //                     strFactory = "\u001b[31mFATAL\u001b[0m";
        //                     break;

        //                 // Default Print case (-1)
        //                 default:
        //                     break;
        //                 }

        //                 if (!print(strFactory.c_str(), strFactory.size()))
        //                     return -1;
        //                 written += len;
        //                 break;
        //             }

        //         // Base 10 Intergers
        //         case 'd':
        //             {
        //                 format++;
        //                 int d = (int) va_arg(parameters, int);
        //                 if (!maxrem) {
        //                     // TODO: Set errno to EOVERFLOW.
        //                     return -1;
        //                 }
        //                 std::string s = std::to_string(d);
        //                 if (!print(s.c_str(), s.size()))
        //                     return -1;
        //                 written++;
        //                 break;
        //             }

        //         case 'x':
        //             {
        //                 format++;
        //                 int temp = (int) va_arg(parameters, int);                        
        //                 buffer = new char[sizeof(int)];
        //                 memset(buffer, 0x00, sizeof(int));
        //                 sprintf(buffer, "%x", temp);
        //                 // itoa(temp, buffer, 16);
        //                 if (!print(buffer, sizeof(temp)))
        //                     return -1;
        //                 written++;
        //                 break;
        //             }
                
        //         default:
        //             {
        //                 format = format_begun_at;
        //                 len = strlen(format); // Change this
        //                 if (maxrem < len) {
        //                     // TODO: Set errno to EOVERFLOW.
        //                     return -1;
        //                 }
        //                 if (!print(format, len))
        //                     return -1;
        //                 written += len;
        //                 format += len;
        //                 break;
        //             }
        //     }
        // }

        // va_end(parameters);
        // return written;
        return 0;
    }
};