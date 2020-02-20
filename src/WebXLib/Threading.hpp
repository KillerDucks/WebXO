#include <stdio.h>
#include <string.h>
#include <thread>
#include <vector>

#ifndef WEBX_THREAD_H
#define WEBX_THREAD_H

namespace WebX
{
    class mThread
    {

    private:
        // Functions
        void ErrorHandler();

        // Data
        int maxThreads = 0;
        std::vector<std::thread::id> _threadIDs;
        std::vector<std::thread> _threads;
    public:
        // Constructors
        mThread(int _maxThreads = 5);
        ~mThread();

        // Thread a function
        void CreateThread(void (*f)());
    };    

}

#endif