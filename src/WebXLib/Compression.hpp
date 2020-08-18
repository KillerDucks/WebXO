// Compression for LibWebX

// Include the common headers
#include "Common.hpp"

// zlib for that sweet gzip compession
#include <zlib.h>

#ifndef WEBX_COMPRESSION_H
#define WEBX_COMPRESSION_H

namespace WebX
{
    class Compression
    {
    private:
        /* data */
    public:
        Compression(/* args */);
        ~Compression();
        
        // Deflate a buffer via a ptr
        CompBuffer DeflateFile(std::string filename);
    };    
}

#endif //WEBX_COMPRESSION_H