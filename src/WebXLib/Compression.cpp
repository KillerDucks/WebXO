#include "Compression.hpp"

namespace WebXO
{
    Compression::Compression(/* args */)
    {
    }
    
    Compression::~Compression()
    {
    }

    CompBuffer Compression::DeflateFile(std::string filename)
    {
        // Variables !!!
        char* sBuffer;
        char* dBuffer;
        std::fstream fsFileStreamer;
        size_t szFileIN = 0;
 
        // Open a File Stream
        // printf("Opening File to Compress\n");
        fsFileStreamer.open(filename, std::ios::binary | std::ios::in);
        if(fsFileStreamer.good())
        {
            // printf("File to Compress is good !!!\n");
            // File Stream is good
            fsFileStreamer.seekg(0, fsFileStreamer.end);
            szFileIN = fsFileStreamer.tellg();
            fsFileStreamer.seekg(0, fsFileStreamer.beg); 
            
            sBuffer = new char[szFileIN + 1];
            fsFileStreamer.read(sBuffer, szFileIN);

            fsFileStreamer.close();
        }

        // Calculate the deflate size
        uLong cSize = compressBound((uLong) szFileIN);
        uLong dSize = cSize;

        // Allocate the Destination Buffer
        dBuffer = new char[cSize];


        // Check if the source buffer is not garbage
        if(strlen(sBuffer) != (szFileIN + 1))
        {
            // Deflate the file
            compress2((Bytef*)dBuffer, &dSize, (Bytef*)sBuffer, szFileIN, Z_BEST_COMPRESSION);
        }
        else
        {
            printf("SOURCE BUFFER HAS FAILED !! \n\n");
            printf("Filename: %s\n", filename);
            std::terminate();
            return CompBuffer((char*)"NULL", -1);
        }
        

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
}