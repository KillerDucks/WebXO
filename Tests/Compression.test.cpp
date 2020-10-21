#include <gtest/gtest.h>

// Include in the Class to test
#include "../src/WebXLib/Compression.hpp"

// Tests the compression of a file [SIZE]
TEST(Compression, DeflateFile_Size) {
    WebXO::Compression compresstionTest;
    CompBuffer testBuffer = compresstionTest.DeflateFile("./assets/deflateME.txt");
    EXPECT_EQ(33, testBuffer.second);
}

// Tests the compression of a file [BUFFER]
TEST(Compression, DeflateFile_CmpFile) {
    WebXO::Compression compresstionTest;
    CompBuffer testBuffer = compresstionTest.DeflateFile("./assets/deflateME.txt");

    char* sBuffer;
    int fLength = 0;
    std::fstream fss;
    fss.open("./assets/deflateME.txt.zz");
    if(fss.good())
    {
        // File is good
        fss.seekg(0, fss.end);
        fLength = fss.tellg();
        fss.seekg(0, fss.beg); 
            
        sBuffer = new char[fLength];
        fss.read(sBuffer, fLength);

        fss.close();
    }
    EXPECT_EQ(fLength, testBuffer.second);
}

// Tests the compression of a Buffer
TEST(Compression, DelfateBuffer) {
    std::string sBuffer("THIS IS A TEST STRING BUFFER");
    WebXO::Compression compresstionTest;
    CompBuffer testBuffer = compresstionTest.DeflateBuffer((char*)sBuffer.c_str(), sBuffer.length());
    EXPECT_EQ(34, testBuffer.second);
}