#include <cstdio>

// Includes
#include "WebXLib/Sockets.hpp"
#include "WebXLib/Directory.hpp"

int main()
{
    // WebX::Sockets sock;
    // sock.Listen();
    WebX::Directory dir((char*)"./");
    dir.GetWebFiles();
    return 0;
}