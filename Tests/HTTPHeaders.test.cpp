#include <gtest/gtest.h>

// Include in the Class to test
#include "../src/WebXLib/HTTPHeaders.hpp"

// Tests the basic core HTTP Header Generation
TEST(HTTPHeaders, General_Header) {
    using namespace WebXO;
    hGeneral gHeader;
    gHeader.Date = gHeader.GetCurrentTime();
    EXPECT_EQ("Connection: close", gHeader.Connection);
}

// Tests the basic core HTTP Entity Generation
TEST(HTTPHeaders, Entity_Header) {
    using namespace WebXO;
    hEntity eHeader;    
    EXPECT_EQ("Allow: GET, POST, HEAD\r\nContent-Type: text/html;\r\nContent-Language: en-GB, en-US\r\n\r\n", eHeader.ReturnHeader());
}

// [TODO] Tests the basic HTTP Reponse Header Generation
