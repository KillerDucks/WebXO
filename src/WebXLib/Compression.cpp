#include "Compression.hpp"
#include "IO.hpp"

namespace WebXO
{
    CompBuffer Compression::DeflateFile(std::string filename)
    {
        // Variables !!!
        char* dBuffer;        

        CompBuffer sBuffer = IO::ReadFile(filename);

        if(sBuffer.second == -1)
        {
            printf("[FATAL] [ERROR] File Stream HAS FAILED !! \n\n");
            printf("[FATAL] [ERROR] Filename: %s\n", filename.c_str());
            // std::terminate();
            return CompBuffer((char*)"NULL", -1);
        }
        

        // Calculate the deflate size
        uLong cSize = compressBound((uLong) sBuffer.second);
        uLong dSize = cSize;

        // Allocate the Destination Buffer
        dBuffer = new char[cSize];


        // Deflate the file
        compress2((Bytef*)dBuffer, &dSize, (Bytef*)sBuffer.first, sBuffer.second, Z_BEST_COMPRESSION);
        

        // [DEBUG] [OPTIONAL] Store Compressed files, maybe useful for a cache
        // char* outFile = new char[filename.size() + 4];
        // sprintf(outFile, "%s.gz", filename.c_str());

        // printf("Uncompressed Size [%ld] !!!\n", szFileIN);
        // printf("Compressed Size [%ld] !!!\n", dSize);

        // fsFileStreamer.open(outFile, std::ios::binary | std::ios::out);
        // if(fsFileStreamer.good())
        // {
        //     // printf("File stream OUT is good !!!\n");
        //     // File Stream is good
        
        //     fsFileStreamer.write(dBuffer, dSize);

        //     fsFileStreamer.close();
        // }
        // printf("DONE !!!\n");

        return CompBuffer(dBuffer, dSize);
    }

    CompBuffer Compression::DeflateBuffer(char* sBuffer, size_t szFileIN)
    {
        // Variables !!!
        char* dBuffer;

        // Calculate the deflate size
        uLong cSize = compressBound((uLong) szFileIN);
        uLong dSize = cSize;

        // Allocate the Destination Buffer
        dBuffer = new char[cSize];

        // Deflate the file
        compress2((Bytef*)dBuffer, &dSize, (Bytef*)sBuffer, szFileIN, Z_BEST_COMPRESSION);


        return CompBuffer(dBuffer, dSize);
    }
}