# WebXO Current TODO's

+ Logarithm
    + [HIGH] [FIX] [CURRENT] [URGENT] Fix overhead of print(....)
        + Possible to reduce the overhead by keeping it disabled as calls will be optimised out
        + Another consideration will be to thread the print
            + Queue via a global buffer removing the need to create an instance per class
                > 
                ```cpp
                std::vector<std::string> vMessageQueue;

                .... Logarithm::QueuePrint()
                {
                    ....
                    if(vMessageQueue.length() == SOME_GLOBAL_LIMIT)
                    {
                        // Send off the bulk Messages to the spool printer
                        std::thread(..., ..., SomeStaticPrintFunction, std::ref(vMessageQueue));
                    }
                }
                ```
        + Another idea is to write the logs to memory and then batch flush the buffer to disk
                
+ HTTP / Socket | Resolved in WebXO v1.2.1 PATCH
    + [RESOLVED] [~~URGENT~~] ~~Solve the video transmission problem~~
        + [~~UPDATE~~] ~~Problem seem to be with the "Content-Type" header~~
            + [~~UPDATE~~] ~~Origin of the issue is close/is the point where "Content-Language" is appended~~
                + [~~CURRENT~~] [~~INVESTIGATING~~] ~~Working on.~~

+ HTTP / HTTP Headers    
    + [LOW] Impl the "Vary" header to allow future requests to cache the response from the server

+ HTTP / HTTP Headers | Resolved in WebXO v1.3.1 PATCH    
    + [RESOLVED] [~~LOW~~] [~~CURRENT~~] ~~Fix the "Content-Length" issue~~

+ HTTP / Path Detection
    + [HIGH] [CURRENT] [URGENT] [REVIEW] Assets called from non-root folders are not directing to the correct place
        + [NOTE] This maybe working as expected
            + [NOTE] Suppression of sub-domain folders from global root might need to be considered

+ HTTP / Compression
    + [REVIEW] [MEDIUM] Look into converting from deflate into gzip for the main compression method

+ HTTP / Interception | Resolved in WebXO v1.5.0 PATCH
    + [RESOLVED] [~~HIGH~~] Current impl does not comply to the accept ranges from the client
        + [~~NOTE~~] The buffer should be loaded and the file path set to ``-1`` or similar, this will allow the current control flow to be used

+ Interception / Core | Resolved in WebXO v1.3.4 PATCH
    + [~~REVIEW~~] [~~LOW~~] ~~Still need a PoC impl to test out basic functionality !!!!~~

+ CMake | Resolved in WebXO v1.5.0 PATCH
    + [RESOLVED] [~~REVIEW~~] [~~LOW~~] The installation of supporting assets should be considered
        + [~~NOTE~~] Minimal is just adding auto install for ``ErrorPages``

+ EntryPoint / CLI Options + Sockets / Threading
    + [LOW] The dis/enable thread option currently does nothing
        + [NOTE] This requires a restructure of the ``WebXO::Sockets::Listen`` method