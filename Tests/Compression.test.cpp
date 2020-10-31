#include <gtest/gtest.h>

// Include in the Class to test
#include "../src/WebXLib/Compression.hpp"
#include "../src/WebXLib/IO.hpp"

// Tests the compression of a file [SIZE]
TEST(Compression, DeflateFile_Size) {
    CompBuffer testBuffer = WebXO::Compression::DeflateFile("./assets/deflateME.txt");
    EXPECT_EQ(33, testBuffer.second);
}

// Tests the compression of a file [BUFFER]
TEST(Compression, DeflateFile_CmpFile) {   
    CompBuffer testBuffer = WebXO::Compression::DeflateFile("./assets/deflateME.txt");
    CompBuffer compressedFile = WebXO::IO::ReadFile("./assets/deflateME.txt.zz");
    EXPECT_EQ(compressedFile.second, testBuffer.second);
}

// Tests the compression of a Buffer
TEST(Compression, DelfateBuffer) {
    std::string sBuffer("THIS IS A TEST STRING BUFFER");    
    CompBuffer testBuffer = WebXO::Compression::DeflateBuffer((char*)sBuffer.c_str(), sBuffer.length());
    EXPECT_EQ(34, testBuffer.second);
}