# WebXO Current TODO's

+ Logarithm
    + [FIX] [CURRENT] Fix overhead of print(....)
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
