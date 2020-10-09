#include <gtest/gtest.h>

// #include <unistd.h>

// Include in the Class to test
#include "../src/WebXLib/VirtualHosts.hpp"

// Tests Parsing and Querying of a vhost.txt file
TEST(VirtualHosts, Parse) {
    WebXO::VirtualHosts vhost("./assets/vhosts.txt"); 
    EXPECT_EQ(std::string("/GTest"), vhost.Query("GTest.localhost"));
}