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

        void gzipTest();
        void gzInflate();
    };    
}

#endif //WEBX_COMPRESSION_H