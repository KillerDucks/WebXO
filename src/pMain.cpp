#include <cstdio>
// #include <thread>


// Includes
#include "WebXLib/Sockets.hpp"
#include "WebXLib/Directory.hpp"
#include "WebXLib/Threading.hpp"

void printa()
{
    printf("[%s] THREAD WORK", std::this_thread::get_id());
}

int main()
{

    // WebX::Sockets sock;
    // sock.Listen();
    // std::regex r("\\.(^[png]|[html]|[js]|[css])+$");
    // std::regex findFile("(index.html)");
    // WebX::Directory dir((char*)"./", r);
    // dir.ScanDir(findFile);

    // std::regex r("\\.(^[png]|[html]|[js]|[css])+$");
    // // Init the Directory class
    // WebX::Directory dir((char*)"./", r);
    // Init the HTTP class
    // WebX::HTTP http;
    // // Init the Sockets class
    // WebX::Sockets socks(http, (int)8080);

    // while (true)
    // {
    //     socks.Listen();
    // }
    WebX::mThread t(5);
    t.CreateThread(printa);
    // std::thread th(printa);

    return 0;
}