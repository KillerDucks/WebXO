// Compression for LibWebX

// Include the common headers
#include "Common.hpp"

// zlib for that sweet gzip compession
#include <zlib.h>

#include <assert.h>

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

        void compress_memory(void *in_data, size_t in_data_size, std::vector<uint8_t> &out_data);
        void test();
        void simple();
    };    
}

#endif //WEBX_COMPRESSION_H