#include "Compression.hpp"

namespace WebX
{
    Compression::Compression(/* args */)
    {
    }
    
    Compression::~Compression()
    {
    }

    void Compression::gzipTest()
    {
        // Vairaibles !!!
        std::string fileIN  = "./gzip/sample.html";
        std::string fileOUT = "./gzip/sample.gz";

        std::vector<char> vBuffer;

        char* cBuffer;
        char* pBuffer;
        std::fstream fsFileStreamer;
        size_t szFileIN = 0;
        unsigned long uFileOUT = 0;
        int iResult = 0;

        z_stream gstream;
        memset(&gstream, 0, sizeof(gstream));

        // Open a File Stream
        printf("Opening File to Compress\n");
        fsFileStreamer.open(fileIN, std::ios::binary | std::ios::in);
        if(fsFileStreamer.good())
        {
            printf("File to Compress is good !!!\n");
            // File Stream is good
            fsFileStreamer.seekg(fsFileStreamer.end);
            szFileIN = fsFileStreamer.tellg();
            fsFileStreamer.seekg(fsFileStreamer.beg); 
            
            cBuffer = new char[szFileIN];
            fsFileStreamer.read(cBuffer, szFileIN);

            fsFileStreamer.close();
        }

        printf("Starting String Output Size: [%ld]\n", szFileIN);

        printf("Compress Init !!!\n");
        iResult = deflateInit2(&gstream, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 9, Z_DEFAULT_STRATEGY);

        if(iResult != Z_OK)
        {
            printf("Compress Init Error !!!\n");    
        }
        
        pBuffer = new char[szFileIN];

        gstream.next_in = reinterpret_cast<Bytef*>(cBuffer);
        gstream.avail_in = szFileIN;

        std::string sOutput;
        int zResult = 0;
        printf("Compress Starting !!!\n");
        do
        {            
            gstream.next_out = reinterpret_cast<Bytef*>(pBuffer);
            gstream.avail_out = sizeof(pBuffer);

            zResult = deflate(&gstream, Z_FINISH);  
            for (size_t i = 0; i < sizeof(pBuffer); i++)
            {
                if(pBuffer[i] == '\0')
                {
                    printf("HIT!!\n");
                    vBuffer.push_back(' ');
                }
                else
                {
                    vBuffer.push_back(pBuffer[i]);
                }
                
            }
            
            // if (sOutput.size() < gstream.total_out) {
            //     // append the block to the output string
            //     sOutput.append(pBuffer, gstream.total_out - sOutput.size());
            // }
        } while (zResult == Z_OK);        

        printf("Compress End !!!\n");

        printf("vBuffer Size: [%ld]\n", vBuffer.size());

        if(zResult != Z_STREAM_END)
        {
            // Error
            printf("zlib error on compression: [%s]\n", "ERROR");
            return;
        }

        printf("Compressed String Output Size: [%ld]\n", sOutput.size());

        fsFileStreamer.open(fileOUT, std::ios::binary | std::ios::out);
        if(fsFileStreamer.good())
        {
            printf("File stream OUT is good !!!\n");
            // File Stream is good
        
            for(auto c : vBuffer)
            {
                if(c == ' ')
                {
                    fsFileStreamer.write((char*)"\0", 1);
                }
                else
                {
                    fsFileStreamer.write(&c, sizeof(c));
                }
                
            }

            fsFileStreamer.close();
        }
        printf("DONE !!!\n");

        return;
    }

    void Compression::gzInflate()
    {
        std::string fileOUT  = "./gzip/sample.inflate.html";
        std::string fileIN = "./gzip/sample.gz";

        char* cBuffer;
        char* pBuffer;
        unsigned long szFileIN = 0;
        std::fstream fsFileStreamer;

        fsFileStreamer.open(fileIN, std::ios::binary | std::ios::in);
        if(fsFileStreamer.good())
        {
            printf("File to Inflate is good !!!\n");
            // File Stream is good
            fsFileStreamer.seekg(fsFileStreamer.end);
            szFileIN = fsFileStreamer.tellg();
            fsFileStreamer.seekg(fsFileStreamer.beg); 
            
            cBuffer = new char[szFileIN];
            fsFileStreamer.read(cBuffer, szFileIN);

            fsFileStreamer.close();
        }

        printf("Starting String Output Size: [%ld]\n", std::string(cBuffer).size());

        std::string strInData(cBuffer);

        z_stream zs;                        // z_stream is zlib's control structure
        memset(&zs, 0, sizeof(zs));

        if (inflateInit2(&zs, 15 + 16) != Z_OK)
            throw(std::runtime_error("inflateInit failed while decompressing."));

        zs.next_in = (Bytef*)strInData.data();
        zs.avail_in = strInData.size();

        int ret = 0;
        char* outbuffer = new char[szFileIN * 2];
        memset(&outbuffer, 0, sizeof(outbuffer));

        std::string outstring;

        // get the decompressed bytes blockwise using repeated calls to inflate
        do {
            zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = sizeof(outbuffer);

            ret = inflate(&zs, 0);

            if (outstring.size() < zs.total_out) {
                outstring.append(outbuffer,
                                zs.total_out - outstring.size());
            }

        } while (ret == Z_OK);

        inflateEnd(&zs);

        // an error occurred that was not EOF
        if (ret != Z_STREAM_END) 
        {
            printf("zlib error on compression: [%s]\n", zs.msg);
            return;
        }

        fsFileStreamer.open(fileOUT, std::ios::binary | std::ios::out);
        if(fsFileStreamer.good())
        {
            printf("File stream OUT is good !!!\n");
            // File Stream is good
        
            fsFileStreamer.write(outbuffer, sizeof(outbuffer));

            fsFileStreamer.close();
        }
        printf("DONE !!!\n");

        return;
    }
}