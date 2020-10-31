// Compression for LibWebX

// Include the common headers
#include "Common.hpp"

// zlib for that sweet gzip compression
#include <zlib.h>

#ifndef WEBX_COMPRESSION_H
#define WEBX_COMPRESSION_H

namespace WebXO
{
    namespace Compression
    {        
        // Deflate a buffer via a ptr
        CompBuffer DeflateFile(std::string filename);

        CompBuffer DeflateBuffer(char* sBuffer, size_t szFileIN);
    };
}

#endif //WEBX_COMPRESSION_H