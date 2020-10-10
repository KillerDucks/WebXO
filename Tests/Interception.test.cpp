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
    std::tuple<WebXO::HTTPReq, CompBuffer> test = intercept.HookSync(WebXO::HTTPReq(), [=](WebXO::HTTPReq &req) -> CompBuffer 
                            { 
                                // Manipulate the request
                                req.host = std::string("googletest");
                                return CompBuffer((char*)"TEST", -2);
                            });
    EXPECT_EQ(std::string("googletest"), std::get<0>(test).host);
}