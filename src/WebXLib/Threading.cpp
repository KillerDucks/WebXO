#include "Threading.hpp"

namespace WebX
{
    mThread::mThread(int _maxThreads)
    {
        this->maxThreads = _maxThreads;
    }

    mThread::~mThread()
    {
        // Check if all threads can be joined back
        if(this->_threadIDs.size() != 0)
        {
            for(auto & th : this->_threads)
            {
                // Try and join the threads back
                while(!th.joinable());
                th.join();
            }
        }
    }

    void mThread::CreateThread(void (*f)())
    {
        // if(this->_threadIDs.size() == this->maxThreads)
        // {
            std::thread th((*f));
            // this->_threads.emplace_back(std::move(th));
            // this->_threadIDs.emplace_back(this->_threads.back().get_id());
        // }
        // printf("Unable to create any more threads\n");
    }
};