#include <cstdio>
// #include <thread>

// Includes
#include "WebXLib/Sockets.hpp"
#include "WebXLib/HTTP.hpp"
#include "WebXLib/Directory.hpp"
#include "WebXLib/Logarithm.hpp"
// #include "WebXLib/Threading.hpp" // Needs Debugging

int main()
{

    WebX::Sockets::Settings sockSettings(true , 2);

    std::regex r("\\.(^[png]|[html]|[js]|[css])+$");
    // // Init the Directory class
    WebX::Directory dir((char*)"./", r);
    // Init the HTTP class
    WebX::HTTP http("./ExampleSite");
    // // Init the Sockets class
    WebX::Sockets socks(http, (int)8080, sockSettings);

    socks.Listen();

    // while (true)
    // {
    //     socks.Listen();
    // }

    return 0;
}