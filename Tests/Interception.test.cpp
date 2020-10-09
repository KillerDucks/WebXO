#include <gtest/gtest.h>

// Include in the Class to test
#include "../src/WebXLib/Interception.hpp"
#include "../src/WebXLib/HTTPHeaders.hpp"

// Tests Parsing and Querying of a vhost.txt file
TEST(Interception, CompBuffer) {
    WebXO::InterceptSettings _iSetting = WebXO::InterceptSettings();
    WebXO::Interception intercept;     
    std::tuple<WebXO::HTTPReq, CompBuffer> test = intercept.HookSync(WebXO::HTTPReq(), _iSetting.callback);
    EXPECT_EQ(-2, std::get<1>(test).second);
}