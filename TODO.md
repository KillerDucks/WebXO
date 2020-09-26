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
+ HTTP / Socket | Resolved in WebXO v1.2.1 PATCH
    + [RESOLVED] [~~URGENT~~] Solve the video transmission problem
        + [UPDATE] Problem seem to be with the "Content-Type" header
            + [UPDATE] Origin of the issue is close/is the point where "Content-Language" is appended
                + [~~CURRENT~~] [~~INVESTIGATING~~] ~~Working on.~~

+ HTTP / HTTP Headers    
    + [LOW] Impl the "Vary" header to allow future requests to cache the response from the server
    + [RESOLVED] [~~LOW~~] [~~CURRENT~~] ~~Fix the "Content-Length" issue~~

+ HTTP / Path Detection
    + [HIGH] [CURRENT] [URGENT] [REVIEW] Assets called from non-root folders are not directing to the correct place
        + [NOTE] This maybe working as expected
            + [NOTE] Suppression of sub-domain folders from global root might need to be considered

+ HTTP / Compression
    + [REVIEW] [MEDIUM] Look into converting from deflate into gzip for the main compression method

+ Interception / Core
    + [REVIEW] [LOW] Still need a PoC impl to test out basic functionality !!!!