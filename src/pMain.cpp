// Includes
#include "WebXLib/Sockets.hpp"
#include "WebXLib/HTTP.hpp"
#include "WebXLib/Directory.hpp"
#include "WebXLib/Compression.hpp"
#include "WebXLib/HTTPHeaders.hpp"
#include "WebXLib/Interception.hpp"

typedef struct Options 
{
    Options() : basePath("./ExampleSite"), port(8080), nThreads(4), threading(false)
    {}
    std::string     basePath;
    int             port;
    int             nThreads;
    bool            threading;    
} OPTIONS;

Options ParseCLIOptions(std::vector<std::string> const vec);

void PrintHello(std::map<std::string, char*> info)
{
    printf("Hello World\n");
    printf("MAP Value [ExampleKey]: %s\n", info.at("ExampleKey"));
}

int main(int argc, char* argv[])
{
    // [TODO] Parse the CLI Args
    std::vector<std::string> vArgs(argv + 1, argv + argc + ! argc);
    Options optsCLI = ParseCLIOptions(vArgs);


    // // [DEBUG] [HALT] Testing Compression
    // WebX::Compression zippy;
    // zippy.simple();
    // zippy.test();
    // zippy.gzipTest();
    // zippy.gzInflate();
    // std::terminate();

    // [DEBUG] [CURRENT] Testing Interception
    // std::terminate();

    // Setup the Socket Server Settings
    WebX::Sockets::Settings sockSettings(optsCLI.threading , optsCLI.nThreads);
    // Init the HTTP class + Set the base path of the HTTP Files
    WebX::HTTP http(optsCLI.basePath);
    // Init the Sockets class
    WebX::Sockets socks(http, optsCLI.port, sockSettings);

    // Start the Socket Server
    socks.Listen();

    // [TODO] Break this out into a thread so the CLI is still usable to perfrom admin activies
    // std::thread th_serverThread(WebX::Sockets::Listen, &socks);
    // th_serverThread.join();

    return 0;
}

Options ParseCLIOptions(std::vector<std::string> const vec)
{
    // Variables !!!
    Options optsCLI = Options();

    // Verbose Printing
    printf("CLI Options\n");

    // Itterate through all the args given
    for(auto opt : vec)
    {
        // Split the Args and rip out the values + error checking (maybe)
        std::string tmpOpt = opt.substr(0, opt.find("="));
        std::string tmpValue = opt.substr(opt.find("=")  + 1, opt.size());

        // Check if we have any options
        if(opt == "--thread")
        {
            optsCLI.threading = true;
        }
        else if(tmpOpt == "--nthreads")
        {
            optsCLI.nThreads = atoi(tmpValue.c_str());
        }
        else if(tmpOpt == "--basepath")
        {
            optsCLI.basePath = tmpValue;
        }
        else if(tmpOpt == "--port")
        {
            optsCLI.port = atoi(tmpValue.c_str());
        }
        else if(tmpOpt == "--help")
        {
            printf("WebXOServer [Options]....\n");
            printf("Options:\n");
            printf("\t--thread\n");
            printf("\t\tIf set this will enable multithreading support where avaliable\n");
            printf("\n\t--nthreads\n");
            printf("\t\tSets the number threads that the server will try to use if --thread is used\n");
            printf("\n\t--basepath\n");
            printf("\t\tIf set this will see the root path to server HTTP requests from\n");
            printf("\n\t--port\n");
            printf("\t\tSets the port of the server to listen for clients on, (Default: 8080)\n");
            printf("\n\t--help\n");
            printf("\t\tShows this message :P\n");
            exit(1);
        }
    }

    printf("Threads: %s\n", (optsCLI.threading) ? "Enabled" : "Disabled");
    printf("Number of Threads: %d\n", optsCLI.nThreads);
    printf("Base Path: %s\n", optsCLI.basePath.c_str());
    printf("Server Listen Port: %d\n", optsCLI.port);

    return optsCLI;
}