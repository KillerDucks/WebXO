#include <gtest/gtest.h>

// Include in the Class to test
#include "../src/WebXLib/Interception.hpp"
#include "../src/WebXLib/HTTPHeaders.hpp"

// Tests Interception
TEST(Interception, CompBuffer_Int) {
    WebXO::InterceptSettings _iSetting = WebXO::InterceptSettings();
    WebXO::Interception intercept;     
    std::tuple<WebXO::HTTPReq, CompBuffer> test = intercept.HookSync(WebXO::HTTPReq(), _iSetting.callback);
    EXPECT_EQ(-2, std::get<1>(test).second);
}

TEST(Interception, CompBuffer_Char) {
    WebXO::InterceptSettings _iSetting = WebXO::InterceptSettings();
    WebXO::Interception intercept;     
    std::tuple<WebXO::HTTPReq, CompBuffer> test = intercept.HookSync(WebXO::HTTPReq(), _iSetting.callback);
    EXPECT_EQ((char*)"TEST", std::get<1>(test).first);
}


TEST(Interception, HTTPReq_Manipulation) {
    WebXO::Interception intercept;     
    WebXO::HTTPReq hReq;
    // Set the connection key to something else
    hReq.host = std::string("NOTTHIS");
    // Run the interceptor and alter the key via a lambda
    std::tuple<WebXO::HTTPReq, CompBuffer> test = intercept.HookSync(hReq, [=](WebXO::HTTPReq &req) -> CompBuffer 
                            { 
                                // Manipulate the request to something we can look for (aka the host)
                                req.host = std::string("googletest");
                                // Return a dummy CompBuffer as we dont need it
                                return CompBuffer((char*)"TEST", -2);
                            });
    // Compare the result
    EXPECT_EQ(std::string("googletest"), std::get<0>(test).host);
}