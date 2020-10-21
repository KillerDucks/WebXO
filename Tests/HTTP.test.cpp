#include <gtest/gtest.h>

// Include in the Class to test
#include "../src/WebXLib/HTTP.hpp"
#include "../src/WebXLib/HTTPHeaders.hpp"

// Tests the basic core HTTP Header Generation
TEST(HTTP, GetRequestedFile) {
    using namespace WebXO;
    HTTPReq hReq;
    // This can be set to anything as we are trying to invoke a 404 error
    hReq.requestType = std::string("RANDOM");
    HTTP http(std::string("./assets"));    
    EXPECT_EQ(3905, http.GetRequestedFile(hReq).second);
}