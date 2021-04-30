// This file contains simple file IO functions
#pragma once

#include "Common.hpp"

namespace WebXO
{
    namespace IO
    {
        inline CompBuffer ReadFile(std::string filename)
        {
            char* buffer;
            int size = 0;
            std::fstream fileStream;

            fileStream.open(filename, std::ios::binary | std::ios::in);

            if(fileStream.good())
            {
                fileStream.seekg(0, fileStream.end);
                size = fileStream.tellg();
                fileStream.seekg(0, fileStream.beg);

                buffer = new char[size + 1];

                fileStream.read(buffer, size);

                fileStream.close();

                return CompBuffer(buffer, size);
            }
            fileStream.close();

            return CompBuffer((char*)"NULL", -1);
        }

        inline bool WriteFile(std::string filename, CompBuffer comBuffer)
        {
            std::fstream fileStream;

            fileStream.open(filename, std::ios::binary | std::ios::out);

            if(fileStream.good())
            {
                fileStream.write(comBuffer.first, comBuffer.second);

                fileStream.close();

                return true;
            }
            fileStream.close();

            return false;
        }

        inline bool WriteFile(std::string filename, int cursor_position, CompBuffer comBuffer)
        {
            std::fstream fileStream;

            fileStream.open(filename, std::ios::binary | std::ios::out);

            if(fileStream.good())
            {
                fileStream.seekg(cursor_position, fileStream.beg);
                
                fileStream.write(comBuffer.first, comBuffer.second);

                fileStream.close();

                return true;
            }
            fileStream.close();

            return false;
        }
    };
};