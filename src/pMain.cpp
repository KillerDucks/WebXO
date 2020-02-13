#include <cstdio>

// Includes
#include "WebXLib/Sockets.hpp"
#include "WebXLib/Directory.hpp"

int main()
{
    WebX::Sockets sock;
    sock.Listen();
    // std::regex r("\\.(^[png]|[html]|[js]|[css])+$");
    // WebX::Directory dir((char*)"./", r);
    // dir.GetWebFiles();
    return 0;
}