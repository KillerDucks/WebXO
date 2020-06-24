// Includes
#include "WebXLib/Sockets.hpp"
#include "WebXLib/HTTP.hpp"
#include "WebXLib/Directory.hpp"

int main(int argc, char** argv)
{

    // Setup the Socket Server Settings
    WebX::Sockets::Settings sockSettings(true , 2);
    // Init the HTTP class + Set the base path of the HTTP Files
    WebX::HTTP http("./ExampleSite");
    // Init the Sockets class
    WebX::Sockets socks(http, (int)8080, sockSettings);

    // Start the Socket Server
    socks.Listen();

    return 0;
}