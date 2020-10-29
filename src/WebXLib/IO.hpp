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
    };
};