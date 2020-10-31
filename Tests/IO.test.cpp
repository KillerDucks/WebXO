#include <gtest/gtest.h>

// Include in the Class to test
#include "../src/WebXLib/IO.hpp"

// Tests IO Read
TEST(IO, ReadFile) {
    using namespace WebXO;
    CompBuffer testRead = IO::ReadFile("./assets/deflateME.txt.zz");
    EXPECT_EQ(33, testRead.second);
}

// Tests IO Writes
TEST(IO, WriteFile) {
    using namespace WebXO;
    bool testWrite = IO::WriteFile("./assets/WRITEFILE.txt", CompBuffer((char*)"Test", strlen((char*)"Test")));
    EXPECT_EQ(true, testWrite);
}