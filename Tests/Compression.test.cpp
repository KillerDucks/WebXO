#include <gtest/gtest.h>

// Include in the Class to test
#include "../src/WebXLib/Compression.hpp"

// Tests Parsing and Querying of a vhost.txt file
TEST(Compression, DeflateFile_Size) {
    WebXO::Compression compresstionTest;
    CompBuffer testBuffer = compresstionTest.DeflateFile("./assets/deflateME.txt");
    EXPECT_EQ(33, testBuffer.second);
}

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