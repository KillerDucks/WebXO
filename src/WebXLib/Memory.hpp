#pragma once

#include "Common.hpp"

#include <sstream>

namespace Utils
{
    namespace Memory
    {
        void Print_Memory(CompBuffer buffer, int limit = -1, bool isPretty = true, int rowLength = 32);
        char Escape_Character_String_Thingy(int escapecharacter);       

        inline std::vector<std::string> s2v(const std::string string_in, char delim)
        {
            std::vector<std::string> vT;
            std::string t;
            std::istringstream tokenStream(string_in);
            while(getline(tokenStream, t, delim))
            {
                // printf("T Value [%s]\n", t.c_str());
                vT.emplace_back(t);
            }
            return vT;
        } 
    };
};