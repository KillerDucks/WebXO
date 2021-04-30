// This file contains simple file Memeory Readout functions

#include "Memory.hpp"

namespace Utils
{
    namespace Memory
    {
        void Print_Memory(CompBuffer buffer, int limit, bool isPretty, int rowLength)
        {
            // Pretty or Not so Pretty
            if(!isPretty)
            {
                printf("%s", buffer.first);
                return;
            }


            // Define the width
            int width = (rowLength <= 0) ? 32 : rowLength;

            // Do a quick check to see if our limit is higher then the rows
            if(limit > (buffer.second) / width) {
                limit = -1;
            }

            // Setup the rows
            const int rows = (limit != -1) ? limit : buffer.second / width;

            // Print Cols
            printf(" RxC | ");
            for(int col = 0; col < width; col ++)
            {
                printf((col == (width - 1)) ? "\033[38;5;135m%02X\033[0m" : "\033[38;5;135m%02X \033[0m", col);
            }
            printf(" |\t\tData\n");

            // Some Decoration
            printf("%-3s", (char*)"---- | ");
            for(int col = 0; col < width; col ++)
            {
                printf("%-3s", (char*)"--");
            }
            printf("|\n");

            // Print Rows
            for(int i = 0; i < rows; i++)
            {
                // Print the Row number
                printf("\033[38;5;135m%04X \033[0m| ", (width / 16) * i);
                // Print the Hex of each byte
                for(int c = 0; c < width; c ++)
                {
                    if(c == (width - 1))
                    {
                        printf("\033[38;5;111m%02X\033[0m", (uint8_t)buffer.first[(i * width) + c]);
                    }
                    else
                    {
                        printf("\033[38;5;111m%02X \033[0m", (uint8_t)buffer.first[(i * width) + c]);
                    }
                    
                }
                // Print the Decoded data for the line
                printf(" | ");
                // Print the ASCII value of each byte
                for(int c = 0; c < width; c ++)
                {
                    char escapeME = Escape_Character_String_Thingy(buffer.first[(i * width) + c]);
                    if(escapeME == '+')
                    {
                        printf("\033[38;5;212m%c\033[0m", buffer.first[(i * width) + c]);
                    }
                    else
                    {
                        printf("%c", escapeME);
                    }
                    
                }
                // Print a newline
                printf("\n");
            }

            // Some Decoration
            printf("%-3s", (char*)"---- | ");
            for(int col = 0; col < width; col ++)
            {
                printf("%-3s", (char*)"--");
            }
            printf("|\n");    

            return;
        }

        char Escape_Character_String_Thingy(int escapecharacter)
        {
            switch(escapecharacter)
            {
                // Pure Escape/Control Characters
                case 0 ... 31:
                    return '.';

                // Anything else that we cant render nicely
                case 127 ... 255:
                    return '.';

                default:
                    if(escapecharacter < 0)
                        return '.';
                    else
                        return '+';
            };
        }
    };
};